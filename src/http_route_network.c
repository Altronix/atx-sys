#include "http_route_network.h"
#include "json.h"
#include "log.h"
#include "network_management.h"
#include <sys/ioctl.h>

static void
get(route_context* ctx)
{
    http_s* http = ctx->context;
    int err, n = 0;
    char* mem;
    FILE* f = fopen(http->network_config, "r");
    if (f) {
        log_info("(SYS) req [%s] 200", http->network_config);
        ioctl(fileno(f), FIONREAD, &n);
        mem = malloc(n + 1);
        assert(mem);
        err = fread(mem, 1, n, f);
        assert(err == n);
        mem[n - 1] = 0;
        http_printf_json(ctx->connection, 200, mem);
        free(mem);
        fclose(f);
    } else {
        log_error("(SYS) req [%s] 404", http->network_config);
        const char* err404 = "{\"error\":\"Not found\"}";
        http_printf_json(ctx->connection, 404, err404);
    }
}

static void
put(route_context* ctx, uint32_t l, const char* b)
{
    int err, i;
    FILE* f = NULL;
    http_s* http = ctx->context;
    const char* r = "{\"error\":\"ok\"}";
    jsmn_parser p;
    jsmn_init(&p);
    jsmntok_t toks[256];
    const jsmntok_t *meth, *ip, *sn, *gw, *hn, *pri, *sec;
    err = jsmn_parse(&p, b, l, toks, 256);
    if ((err > 0) && //
        (meth = json_delve(b, toks, ".network.ipv4.meth")) &&
        (ip = json_delve(b, toks, ".network.ipv4.ip")) &&
        (sn = json_delve(b, toks, ".network.ipv4.sn")) &&
        (gw = json_delve(b, toks, ".network.ipv4.gw")) &&
        (hn = json_delve(b, toks, ".network.ipv4.hn")) &&
        (pri = json_delve(b, toks, ".dashboard.primary")) &&
        (sec = json_delve(b, toks, ".dashboard.secondary"))) {
        if ((f = fopen(http->network_config, "w"))) {
            err = print_network_config_toks(f, b, meth, ip, sn, gw, hn);
            if (!(err < 0)) {
                http_printf_json(ctx->connection, 200, r);
            } else {
                r = strerror(errno);
                log_error("(SYS) file io error [%s]", r);
                http_printf_json(ctx->connection, 500, "{\"error\":\"%s\"}", r);
            }
            fclose(f);
        } else {
            r = strerror(errno);
            log_error("(SYS) file io error [%s]", r);
            http_printf_json(ctx->connection, 500, "{\"error\":\"%s\"}", r);
        }
    } else {
        r = "parser error";
        log_error("(SYS) parser error [%s]", r);
        http_printf_json(ctx->connection, 500, "{\"error\":\"%s\"}", r);
    }
}

void
route_network(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    switch (meth) {
        case HTTP_METHOD_GET: get(ctx); break;
        case HTTP_METHOD_PUT:
        case HTTP_METHOD_POST: put(ctx, l, b); break;
        case HTTP_METHOD_DELETE: break;
    }
}
