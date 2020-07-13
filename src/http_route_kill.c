#include "http_route_kill.h"

void
route_kill(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    http_s* http = ctx->context;
    http->shutdown = true;
}
