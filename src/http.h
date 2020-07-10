#ifndef HTTP_H
#define HTTP_H

#include "private.h"

#include "mongoose.h"
#undef LIST_INIT /* leaky header :( */

#include "env.h"
#include "updater.h"

#include "containers.h"
#define ROUTE_CB_ARGS route_context *, HTTP_METHOD meth, uint32_t, const char *

// Type of request
typedef enum
{
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
} HTTP_METHOD;

typedef struct route_context route_context;
typedef void (*route_cb)(route_context*, HTTP_METHOD, uint32_t, const char*);

typedef struct route_context
{
    route_cb cb;
    void* context;
    struct mg_connection* connection;
    struct http_message* message;
    struct route_context* self;
} route_context;
MAP_INIT_H(routes, route_context);

typedef struct http_s
{
    struct mg_connection* http;
    struct mg_connection* https;
    struct mg_mgr connections;
    struct mg_serve_http_opts serve_opts;
    updater_s updater;
    updater_progress_s updater_progress;
    env_s* env;
    routes_map_s* routes;
} http_s;

void http_init(http_s* http, env_s* env);
void http_deinit(http_s* http);
int http_poll(http_s*, int32_t);
void http_listen(http_s* http, const char* port);
void http_use(http_s* http, const char* path, route_cb, void*);
void http_serve(http_s* http, const char* path);
void http_parse_query_str(
    route_context* c,
    const char* want,
    const char** result,
    uint32_t* l);
void http_broadcast_json(http_s* http, int code, const char* fmt, ...);
void http_printf_json(struct mg_connection*, int, const char*, ...);
void http_printf(struct mg_connection*, int, const char*, const char*, ...);
void http_vprintf(
    struct mg_connection*,
    int,
    const char*,
    uint32_t,
    const char*,
    va_list);
const char* http_error_message(int);
uint32_t http_error_code(int);

#endif /* HTTP_H */
