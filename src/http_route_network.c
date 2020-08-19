#include "http_route_network.h"
#include "log.h"
#include <sys/ioctl.h>
static const char* network = "{"
                             "\"network\":"
                             "{"
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
                             "}"
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

// TODO needs test
static void
get(route_context* ctx)
{
    http_s* http = ctx->context;
    int err, n = 0;
    char* mem;
    FILE* f = fopen(http->network_config, "r");
    if (f) {
        ioctl(fileno(f), FIONREAD, &n);
        mem = malloc(n + 1);
        assert(mem);
        err = fread(mem, 1, n, f);
        assert(err == n);
        http_printf_json(ctx->connection, 200, mem);
        free(mem);
        fclose(f);
    } else {
        const char* err404 = "{\"error\":\"Not found\"}";
        http_printf_json(ctx->connection, 404, err404);
    }
}

static void
put(route_context* ctx, uint32_t l, const char* b)
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
        case HTTP_METHOD_PUT:
        case HTTP_METHOD_POST: put(ctx, l, b); break;
        case HTTP_METHOD_DELETE: del(ctx); break;
    }
}
