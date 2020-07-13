#include "http_route_kill.h"

void
route_kill(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    char resp[256];
    snprintf(
        resp,
        sizeof(resp),
        "{\"error\":200,\"message\":\"%s\"}",
        http_error_message(200));
    http_s* http = ctx->context;
    http->shutdown = true;
    http_printf_json(ctx->connection, 200, resp);
}
