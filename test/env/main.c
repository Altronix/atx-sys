#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libubootenv.h"

#include <setjmp.h>

#include "cmocka.h"

int
main(int argc, char* argv[])
{
    int err = 0;
    const struct CMUnitTest tests[] = {};
    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
