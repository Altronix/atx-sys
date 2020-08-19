// Copyright (c) 2019-2020 Altronix Corp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "http.h"
#include "mock_file.h"
#include "mongoose_mock.h"
#include "private.h"

#include "http_route_network.h"

#include <setjmp.h>

#include <cmocka.h>

static const char* network = "{"
                             "\"network\":"
                             "{"
                             "\"mac\":\"00:CC:DD:EE:00:22:AA\","
                             "\"ipv4\":"
                             "{"
                             "\"ip\":\"192.168.0.66\","
                             "\"sn\":\"255.255.255.0\","
                             "\"gw\":\"192.168.0.1\","
                             "\"meth\":\"STATIC\","
                             "\"hn\":\"foo\""
                             "},"
                             "\"ipv6\":"
                             "{"
                             "\"ip\":\"FE80::9ED2:1EFF:FEAE:CC5C\""
                             "}"
                             "},"
                             "\"dashboard\":"
                             "{"
                             "\"primary\":\"tcp://1.1.1.1:333\","
                             "\"secondary\":\"tcp://1.1.1.1:333\""
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
test_route_network_get(void** context_p)
{
    http_s http;
    memset(&http, 0, sizeof(http_s));
    mongoose_parser_context* parser;
    route_context ctx = { .context = &http };

    spy_file_init();
    mongoose_spy_init();
    spy_file_push_incoming(network, strlen(network));

    route_network(&ctx, HTTP_METHOD_GET, 0, NULL);
    parser = mongoose_spy_response_pop();
    assert_memory_equal(parser->body, network, strlen(network));
    free(parser);

    spy_file_free();
    mongoose_spy_deinit();
}

static void
test_route_network_put(void** context_p)
{
    http_s http;
    memset(&http, 0, sizeof(http_s));
    mongoose_parser_context* outmg;
    spy_file_packet_s* outf;
    route_context ctx = { .context = &http };

    spy_file_init();
    mongoose_spy_init();
    spy_file_push_incoming(network, strlen(network));

    route_network(&ctx, HTTP_METHOD_PUT, strlen(network), network);
    outmg = mongoose_spy_response_pop();
    outf = spy_file_packet_pop_outgoing();
    assert_non_null(outf);
    assert_non_null(outmg);
    spy_file_packet_free(&outf);
    free(outmg);

    spy_file_free();
    mongoose_spy_deinit();
}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_route_network_get),
        cmocka_unit_test(test_route_network_put),
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}

