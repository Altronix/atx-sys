#include "http.h"
#include "http_route_about.h"
#include "http_route_env.h"
#include "http_route_kill.h"
#include "http_route_network.h"
#include "http_route_update.h"
#include "log.h"
#include "updater.h"

#define mg_use(http, path, fn, ctx)                                            \
    mg_register_http_endpoint(http->http, path, fn, ctx)

#define HTTP_FORMAT_HEADERS                                                    \
    "HTTP/1.0 %d \r\n"                                                         \
    "Server: Altronix atx-update Embedded Web Server\r\n"                      \
    "Connection: %s\r\n"                                                       \
    "Content-Type: %s\r\n"                                                     \
    "Content-Length: %d\r\n"                                                   \
    "%s"                                                                       \
    "X-Frame-Options: SAMEORIGIN\r\n"                                          \
    "Content-Security-Policy: script-src 'self' 'unsafe-eval';\r\n"            \
    "\r\n"

// If caller sends us variadic format string with out the length,
// we use this to get the length prior to sending
#define va_get_len(__list, __fmt, result)                                      \
    va_start(__list, __fmt);                                                   \
    result = vsnprintf(NULL, 0, __fmt, __list);                                \
    va_end(__list);

// Hash map of URL's to routes
MAP_INIT_W_FREE(routes, route_context);

const char*
http_error_message(int e)
{
    static const char* ok = "ok";
    static const char* server_error = "server error";
    static const char* client_error = "client error";
    static const char* authorization = "authorization error";
    static const char* not_found = "not found";
    static const char* busy = "try again later";
    static const char* server = "server error";
    int code = http_error_code(e);
    if (code == 200) {
        return ok;
    } else if (code == 400) {
        return client_error;
    } else if (code == 403) {
        return authorization;
    } else if (code == 404) {
        return not_found;
    } else if (code == 500) {
        return server;
    } else if (code == 504) {
        return busy;
    } else {
        return server_error;
    }
}

uint32_t
http_error_code(int e)
{
    int ret = 0;
    switch (e) {
        case -1: ret = 400; break;
        case 0: ret = 200; break;
        case 200:
        case 400:
        case 403:
        case 404:
        case 500:
        case 504: ret = e; break;
    }
    return ret;
}

static inline HTTP_METHOD
get_method(struct http_message* m)
{
    if (m->method.p[0] == 'P') {
        return m->method.p[1] == 'U' ? HTTP_METHOD_PUT : HTTP_METHOD_POST;
    } else if (m->method.p[0] == 'D') {
        return HTTP_METHOD_DELETE;
    } else {
        return HTTP_METHOD_GET;
    }
}

// Write to connection
static void
c_vprintf(
    struct mg_connection* c,
    int code,
    const char* type,
    uint32_t l,
    const char* fmt,
    va_list list)
{
    // Send headers
    mg_printf(c, HTTP_FORMAT_HEADERS, code, "keep-alive", type, l, "");
    mg_vprintf(c, fmt, list);
}

// Write json to connection
static void
c_printf_json(struct mg_connection* c, int code, const char* fmt, ...)
{
    size_t l;
    va_list ap;

    // Send data
    va_get_len(ap, fmt, l);
    va_start(ap, fmt);
    c_vprintf(c, code, "application/json", l, fmt, ap);
    va_end(ap);
}

static void
c_printf(void* connection, int code, const char* type, const char* fmt, ...)
{
    size_t l;
    va_list ap;

    // Send data
    va_get_len(ap, fmt, l);
    va_start(ap, fmt);
    c_vprintf(connection, code, type, l, fmt, ap);
    va_end(ap);
}

static inline bool
is_websocket(const struct mg_connection* c)
{
    return c->flags & MG_F_IS_WEBSOCKET;
}

static inline void
get_addr(const struct mg_connection* c, char* buff, uint32_t sbuff)
{
    mg_sock_addr_to_str(
        &c->sa, buff, sbuff, MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
}

typedef struct foreach_route_check_path_context
{
    const char* path;
    route_context*** found_p;
} foreach_route_check_path_context;

static void
foreach_route_check_path(
    routes_map_s* self,
    void* data,
    const char* test_path,
    route_context** r_p)
{
    ((void)self);
    foreach_route_check_path_context* ctx = data;
    if (!*ctx->found_p) {
        uint32_t plen = strlen(ctx->path), tlen = strlen(test_path);
        if (tlen >= 4 && !(memcmp(&test_path[tlen - 4], "/...", 4))) {
            if (plen > tlen && !(memcmp(test_path, ctx->path, tlen - 4))) {
                *ctx->found_p = r_p;
            }
        }
    }
}

static route_context**
resolve_route(http_s* http, struct mg_str* uri)
{
    static char path[128];
    assert(uri->len < sizeof(path));
    route_context** r_p = NULL;
    snprintf(path, sizeof(path), "%.*s", (uint32_t)uri->len, uri->p);
    foreach_route_check_path_context ctx = { .path = path, .found_p = &r_p };
    r_p = routes_map_get(http->routes, path);
    if (!r_p) routes_map_foreach(http->routes, foreach_route_check_path, &ctx);
    return r_p;
}

static void
process_route(
    route_context** r,
    struct mg_connection* c,
    struct http_message* m)
{
    (*r)->connection = c;
    (*r)->message = m;
    (*r)->cb(*r, get_method(m), m->body.len, m->body.p);
}

static void
process_file(http_s* http, struct mg_connection* c, struct http_message* m)
{
    char b[512];
    const char* root = http->serve_opts.document_root;
    int err;
    unsigned int _p;
    struct stat st;
    struct mg_str _s, _u, _h, path, _q, _f;
    err = mg_parse_uri(m->uri, &_s, &_u, &_h, &_p, &path, &_q, &_f);
    if (err) return;
    snprintf(b, sizeof(b), "%s%.*s", root, path.len, path.p);
    err = stat(b, &st);
    if (err) {
        log_debug("(HTTP) [%s] not found... Serving root", b);
        snprintf(b, sizeof(b), "%s%s", root, "/index.html");
        mg_http_serve_file(c, m, b, mg_mk_str("text/html"), mg_mk_str(""));
    } else {
        log_debug("(HTTP) [%s] found... ", b);
        mg_serve_http(c, m, http->serve_opts);
    }
}

static void
ev_handler(struct mg_connection* c, int ev, void* p, void* user_data)
{
    switch (ev) {
        case MG_EV_POLL: break;
        case MG_EV_ACCEPT: log_trace("%06s %04s", "(HTTP)", "Accept"); break;
        case MG_EV_CONNECT: log_trace("%06s %04s", "(HTTP)", "Connect"); break;
        case MG_EV_RECV: log_trace("%06s %04s", "(HTTP)", "Recv"); break;
        case MG_EV_SEND: log_trace("%06s %04s", "(HTTP)", "Send"); break;
        case MG_EV_CLOSE: {
            char addr[48];
            get_addr(c, addr, sizeof(addr));
            log_info("(HTTP) (%s) Connection close", addr);
        } break;
        case MG_EV_TIMER: log_trace("%06s %04s", "(HTTP)", "Timer"); break;
        case MG_EV_HTTP_REQUEST: {
            http_s* http = user_data;
            struct http_message* m = (struct http_message*)p;
            struct mg_str* path = &m->uri;
            route_context** r = resolve_route(http, path);
            if (r) {
                process_route(r, c, m);
            } else if (http->serve_opts.document_root) {
                process_file(http, c, m);
            } else {
                log_warn("%06s %04s %s [%s]", "(HTTP)", "Req.", "(404)", path);
                c_printf_json(c, 404, "{\"error\":\"%s\"}", "not found");
            }
        } break;
        case MG_EV_HTTP_REPLY: log_trace("%06s %04s", "(HTTP)", "Reply"); break;
        case MG_EV_HTTP_CHUNK: log_trace("%06s %04s", "(HTTP)", "Chunk"); break;
        case MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: {
            char addr[48];
            get_addr(c, addr, sizeof(addr));
            log_info("(HTTP) (%s) Received websocket request...", addr);
        } break;
        case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
        case MG_EV_WEBSOCKET_FRAME:
            log_trace("%06s %04s", "(HTTP)", "Websocket frame");
        case MG_EV_WEBSOCKET_CONTROL_FRAME:
            log_trace("%06s %04s", "(HTTP)", "Websocket ctrl frame");
            break;
        default:
            log_error("%06s %04s %s (%d)", "(HTTP)", "Recv", "Unkown", ev);
            break;
    }
}

static void
updater_progress_status(void* ctx, UPDATER_PROGRESS_STATUS status)
{
    http_s* http = ctx;
    int l;
    char b[512];
    const char* msg = updater_progress_status_message(status);
    log_info("(UPDATER PROGRESS) status %d", status);
    l = snprintf(b, sizeof(b), "{\"type\":\"status\",\"status\":\"%s\"}", msg);
    http_broadcast_json(http, 200, b, l);
}

static void
updater_progress_source(void* ctx, UPDATER_PROGRESS_SOURCE source)
{
    http_s* http = ctx;
    int l;
    char b[512];
    const char* msg = updater_progress_source_message(source);
    log_info("(UPDATER PROGRESS) source %d", source);
    l = snprintf(b, sizeof(b), "{\"type\":\"source\",\"source\":\"%s\"}", msg);
    http_broadcast_json(http, 200, b, l);
}

static void
updater_progress_info(void* ctx, const char* info, uint32_t len)
{
    http_s* http = ctx;
    char b[512];
    int l;
    log_info("(UPDATER PROGRESS) info %.*s", len, info);
    l = snprintf(
        b,
        sizeof(b),
        "{"
        "\"type\":\"info\","
        "\"info\":\"%.*s\""
        "}",
        len,
        info);
    http_broadcast_json(http, 200, b, l);
}

static void
updater_progress_step(
    void* ctx,
    const char* name,
    uint8_t step,
    uint8_t total,
    uint8_t per)
{
    http_s* http = ctx;
    char b[512];
    int l;
    log_info("(UPDATER PROGRESS) step %d", per);
    l = snprintf(
        b,
        sizeof(b),
        "{"
        "\"type\":\"step\","
        "\"step\":%d,"
        "\"total\":%d,"
        "\"percent\":%d"
        "}",
        step,
        total,
        per);
    http_broadcast_json(http, 200, b, l);
}

updater_progress_callbacks_s updater_progress_callbacks = {
    .status = updater_progress_status,
    .source = updater_progress_source,
    .info = updater_progress_info,
    .step = updater_progress_step,
};

void
http_init(http_s* http, env_s* env, const char* nconfig)
{
    memset(http, 0, sizeof(http_s));
    http->env = env;
    http->network_config = nconfig;
    mg_mgr_init(&http->connections, http);
    http->routes = routes_map_create();
    updater_progress_init(
        &http->updater_progress, &updater_progress_callbacks, http);
}

void
http_deinit(http_s* http)
{
    updater_free(&http->updater);
    updater_progress_free(&http->updater_progress);
    mg_mgr_free(&http->connections);
    routes_map_destroy(&http->routes);
}

int
http_poll(http_s* http, int32_t ms)
{
    int err = mg_mgr_poll(&http->connections, ms);
    if (http->env->valid) updater_progress_poll(&http->updater_progress);
    return err;
}

void
http_listen(http_s* http, const char* port)
{
    if (http->http) {
        log_fatal("%10s", "HTTP can only listen to one server at a time!");
        log_fatal("%10s", "Please shutdown HTTP server before listening again");
        assert(http->http);
    }
    log_info("(HTTP) Listening... [http://*:%s]", port);
    http->http = mg_bind(&http->connections, port, ev_handler, http);
    mg_set_protocol_http_websocket(http->http);
    mg_use(http, "/api/v1/update", update_handler, http);
    http_use(http, "/api/v1/env", route_env, http);
    http_use(http, "/api/v1/network", route_network, http);
    http_use(http, "/api/v1/kill", route_kill, http);
    http_use(http, "/api/v1/about", route_about, http);
}

void
http_use(http_s* http, const char* path, route_cb cb, void* context)
{
    route_context* route = malloc(sizeof(route_context));
    assert(route);
    route->cb = cb;
    route->context = context;
    routes_map_add(http->routes, path, &route);
}

void
http_serve(http_s* http, const char* path)
{
    log_info("(HTTP) Serving... [%s]", path);
    http->serve_opts.document_root = path;
}

void
http_parse_query_str(
    route_context* c,
    const char* want,
    const char** result,
    uint32_t* l)
{
    struct mg_str* q = &c->message->query_string;
    if (q) {
        struct mg_str needle = { .p = want, .len = strlen(want) };
        const char *end, *spot = mg_strstr(*q, needle);
        uint32_t wantlen = strlen(want);
        if (spot) {
            if (!(wantlen <= q->len &&                    // bad query str
                  (size_t)spot - (size_t)q->p < q->len && // bad query str
                  (!(spot[wantlen] == '='))))             // bad query str
            {
                spot += wantlen + 1;
                end = memchr(spot, '&', q->len - (spot - q->p));
                *l = end ? end - spot : q->len - (spot - q->p);
                *result = spot;
            } else {
                log_error("(HTTP) Invalid Query String Detected");
                *result = NULL;
                *l = 0;
            }
        } else {
            *result = NULL;
            *l = 0;
        }
    }
}

void
http_broadcast_json(http_s* http, int code, const char* fmt, ...)
{
    char mem[1024];
    struct mg_connection* c;
    int len;
    va_list list;

    va_start(list, fmt);
    len = vsnprintf(mem, sizeof(mem), fmt, list);
    va_end(list);

    if (len) {
        // TODO need __wrap_mg_next() to test broadcast output
        for (c = mg_next(&http->connections, NULL); c != NULL;
             c = mg_next(&http->connections, c)) {
            if (is_websocket(c)) {
                mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, mem, len);
            }
        }
    }
}

void
http_printf_json(
    struct mg_connection* connection,
    int code,
    const char* fmt,
    ...)
{
    size_t l;
    va_list ap;

    // Send data
    va_get_len(ap, fmt, l);
    va_start(ap, fmt);
    c_vprintf(connection, code, "application/json", l, fmt, ap);
    va_end(ap);
}

void
http_printf(
    struct mg_connection* connection,
    int code,
    const char* type,
    const char* fmt,
    ...)
{
    size_t l;
    va_list ap;

    // Send data
    va_get_len(ap, fmt, l);
    va_start(ap, fmt);
    c_vprintf(connection, code, type, l, fmt, ap);
    va_end(ap);
}
