#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libubootenv.h"

#include "libuboot.h"

void*
__wrap_libuboot_iterator(struct uboot_ctx* ctx, void* next)
{
    ((void)ctx);
    return NULL;
}

const char*
__wrap_libuboot_getname(void* entry)
{
    return NULL;
}

const char*
__wrap_libuboot_getvalue(void* entry)
{
    return NULL;
}
