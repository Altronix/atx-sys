#include "http_route_about.h"

#define ABOUT_FMT                                                              \
    "{"                                                                        \
    "\"sid\":\"%s\","                                                          \
    "\"product\":\"%s\","                                                      \
    "\"prjVersion\":\"%s\","                                                   \
    "\"atxVersion\":\"%s\","                                                   \
    "\"webVersion\":\"%s\","                                                   \
    "\"mac\":\"%s\","                                                          \
    "}"

void
route_about(route_context* ctx, HTTP_METHOD meth, uint32_t l, const char* b)
{
    http_printf_json(
        ctx->connection,
        200,
        ABOUT_FMT,
        "linqm5serialnumberhere",
        "linqm5",
        "0.0.1",
        "0.0.1",
        "0.0.1",
        "00:00:00:00:00:00");
}
