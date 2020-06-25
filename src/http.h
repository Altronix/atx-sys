#ifndef HTTP_H
#define HTTP_H

#include "private.h"

#include "mongoose.h"
#undef LIST_INIT /* leaky header :( */

#include "env.h"

#include "containers.h"

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
    struct mg_connection* curr_connection;
    struct http_message* curr_message;
    struct route_context* self;
} route_context;
MAP_INIT_H(routes, route_context);

typedef struct http_s
{
    struct mg_connection* http;
    struct mg_connection* https;
    struct mg_mgr connections;
    struct mg_serve_http_opts serve_opts;
    env_s* env;
    routes_map_s* routes;
} http_s;

void http_init(http_s* http, env_s* env);

#endif /* HTTP_H */
