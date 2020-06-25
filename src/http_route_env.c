#include "http_route_env.h"
#include "env.h"

void
route_env(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
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
