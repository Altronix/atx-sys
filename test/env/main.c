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
                   "\"img_addr\":\"0xd0000\","
                   "\"foo\":\"mtdparts='abc'\""
                   "}";
    uint32_t sz = 4096, expect_sz = strlen(expect);
    char buffer[sz];
    env_s env;
    libubootenv_spy_init();
    libubootenv_spy_push_iter("bootcmd", "console=ttyS0,115200 earlyprintk");
    libubootenv_spy_push_iter("dtb_addr", "0xb0000");
    libubootenv_spy_push_iter("img_addr", "0xd0000");
    libubootenv_spy_push_iter("foo", "mtdparts=\"abc\"");

    env_read(&env, buffer, &sz);
    assert_int_equal(sz, expect_sz);
    assert_memory_equal(buffer, expect, expect_sz);

    libubootenv_spy_deinit();
}

static void
test_read_val_ok(void** context_p)
{
    const char* val;
    env_s env;
    libubootenv_spy_init();
    libubootenv_spy_push_iter("bootcmd", "console=ttyS0,115200 earlyprintk");
    libubootenv_spy_push_iter("dtb_addr", "0xb0000");
    libubootenv_spy_push_iter("img_addr", "0xd0000");

    val = env_read_val(&env, "bootcmd");
    assert_string_equal(val, "console=ttyS0,115200 earlyprintk");
    val = env_read_val(&env, "dtb_addr");
    assert_string_equal(val, "0xb0000");
    val = env_read_val(&env, "img_addr");
    assert_string_equal(val, "0xd0000");

    libubootenv_spy_deinit();
}

static void
test_read_val_err(void** context_p)
{
    const char* val;
    int ret;
    env_s env;
    libubootenv_spy_init();
    libubootenv_spy_push_iter("bootcmd", "console=ttyS0,115200 earlyprintk");
    libubootenv_spy_push_iter("dtb_addr", "0xb0000");
    libubootenv_spy_push_iter("img_addr", "0xd0000");

    val = env_read_val(&env, "foo");
    assert_int_equal(val, NULL);

    libubootenv_spy_deinit();
}

int
main(int argc, char* argv[])
{
    int err = 0;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read),
        cmocka_unit_test(test_read_val_ok),
        cmocka_unit_test(test_read_val_err),
    };
    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
