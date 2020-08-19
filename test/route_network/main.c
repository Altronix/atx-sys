// Copyright (c) 2019-2020 Altronix Corp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "http.h"
#include "mongoose_mock.h"
#include "private.h"

#include <setjmp.h>

#include <cmocka.h>

static void
test_route_env_200(void** context_p)
{}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_route_env_200),
        // cmocka_unit_test(test_http_simple_get),
        // cmocka_unit_test(test_http_simple_query),
        // cmocka_unit_test(test_http_invalid_query)
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}

