#include "http_route_env.h"
#include "env.h"

static void
get(route_context* ctx)
{
    uint32_t keyl;
    const char *keyp = NULL, *val = NULL;
    char key[32];
    http_s* http = ctx->context;
    http_parse_query_str(ctx, "var", &keyp, &keyl);
    if (keyp) {
        snprintf(key, sizeof(keyp), "%.*s", keyl, keyp);
        val = env_read_val(http->env, key);
        if (val) {
            http_printf_json(ctx->connection, 200, "{\"val\":\"%s\"", val);
        } else {
            val = http_error_message(404);
            http_printf_json(ctx->connection, 404, "{\"error\":\"%s\"", val);
        }
    } else {
        uint32_t sz = 4096;
        char buffer[sz];
        env_read(http->env, buffer, &sz);
        http_printf_json(ctx->connection, 200, "%s", buffer);
    }
}

static void
post(route_context* ctx)
{
    // TODO
}

static void
put(route_context* ctx)
{
    // TODO
}

static void
del(route_context* ctx)
{
    // TODO
}

void
route_env(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    switch (meth) {
        case HTTP_METHOD_GET: get(ctx); break;
        case HTTP_METHOD_PUT: put(ctx); break;
        case HTTP_METHOD_POST: post(ctx); break;
        case HTTP_METHOD_DELETE: del(ctx); break;
    }
}
