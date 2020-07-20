// Copyright (c) 2019-2020 Altronix Corp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "http.h"
#include "json.h"
#include "network_management.h"
#include "private.h"

#include <setjmp.h>

#include <cmocka.h>

static void
test_parse_network_config(void** context_p)
{
    const char* b = "{"
                    "\"network\":"
                    "{"
                    "\"ip\":\"192.168.0.44\","
                    "\"sn\":\"255.255.0.0\","
                    "\"gw\":\"192.168.0.100\","
                    "\"meth\":\"static\","
                    "\"hn\":\"sample-network-id\""
                    "}"
                    "}";
    jsmn_value meth, ip, sn, gw, hn;
    int err;
    err = parse_network_config(b, strlen(b), &meth, &ip, &sn, &gw, &hn);
    assert_int_equal(err, 0);
    assert_int_equal(meth.len, 6);
    assert_memory_equal("static", meth.p, meth.len);
    assert_int_equal(hn.len, strlen("sample-network-id"));
    assert_memory_equal("sample-network-id", hn.p, hn.len);
    assert_int_equal(ip.len, strlen("192.168.0.44"));
    assert_memory_equal("192.168.0.44", ip.p, ip.len);
    assert_int_equal(sn.len, strlen("255.255.0.0"));
    assert_memory_equal("255.255.0.0", sn.p, sn.len);
    assert_int_equal(gw.len, strlen("192.168.0.100"));
    assert_memory_equal("192.168.0.100", gw.p, gw.len);
}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_network_config),
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}

