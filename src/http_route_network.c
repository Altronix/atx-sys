#include "http_route_network.h"
#include "log.h"
static const char* network = "{"
                             "\"mac\":\"00:CC:DD:EE:00:22:AA\","
                             "\"ipv4\":"
                             "{"
                             "\"ip\":\"192.168.0.66\","
                             "\"sn\":\"255.255.255.0\","
                             "\"gw\":\"192.168.0.1\","
                             "\"meth\":\"STATIC\""
                             "},"
                             "\"ipv6\":"
                             "{"
                             "\"ip\":\"FE80::9ED2:1EFF:FEAE:CC5C\""
                             "},"
                             "\"dashboard\":"
                             "{"
                             "\"primaryIp\":\"192.168.0.22\","
                             "\"primaryPort\":33333,"
                             "\"primaryTls\":false,"
                             "\"secondaryIp\":\"192.168.0.23\","
                             "\"secondaryPort\":33334,"
                             "\"secondaryTls\":false"
                             "},"
                             "\"certificates\":"
                             "{"
                             "\"primaryTlsCert\":\"\","
                             "\"primaryTlsKey\":false,"
                             "\"secondaryTlsCert\":\"\","
                             "\"secondaryTlsKey\":false"
                             "}"
                             "}";

static void
get(route_context* ctx)
{
    http_printf_json(ctx->connection, 200, network);
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
route_network(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    switch (meth) {
        case HTTP_METHOD_GET: get(ctx); break;
        case HTTP_METHOD_PUT: put(ctx); break;
        case HTTP_METHOD_POST: post(ctx); break;
        case HTTP_METHOD_DELETE: del(ctx); break;
    }
}
