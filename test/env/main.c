#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"

#include "libubootenv.h"

#include <setjmp.h>

#include "cmocka.h"

static void
test_read(void** context_p)
{
    char* expect = "{"
                   "\"bootcmd\":\"console=ttyS0,115200 earlyprintk\","
                   "\"dtb_addr\":\"0xb0000\","
                   "\"img_addr\":\"0xd0000\""
                   "}";
    uint32_t sz = 4096, expect_sz = strlen(expect);
    char buffer[sz];
    env_s env;
    libubootenv_spy_init();
    libubootenv_spy_push_iter("bootcmd", "console=ttyS0,115200 earlyprintk");
    libubootenv_spy_push_iter("dtb_addr", "0xb0000");
    libubootenv_spy_push_iter("img_addr", "0xd0000");

    env_read(&env, buffer, &sz);
    assert_int_equal(sz, expect_sz);
    assert_memory_equal(buffer, expect, expect_sz);

    libubootenv_spy_deinit();
}

int
main(int argc, char* argv[])
{
    int err = 0;
    const struct CMUnitTest tests[] = { cmocka_unit_test(test_read) };
    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
