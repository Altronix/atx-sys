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

#include "containers.h"

void
free_fn(char** ctx_p)
{
    char* ctx = *ctx_p;
    *ctx_p = NULL;
    free(ctx);
}

LIST_INIT(key, char, free_fn);
LIST_INIT(val, char, free_fn);
key_list_s* keys;
val_list_s* vals;
bool g_next;

void
libubootenv_spy_init()
{
    g_next = false;
    keys = key_list_create();
    vals = val_list_create();
}

void
libubootenv_spy_deinit()
{
    key_list_destroy(&keys);
    val_list_destroy(&vals);
}

void
libubootenv_spy_push_iter(const char* in_key, const char* in_val)
{
    char* key = malloc(strlen(in_key));
    char* val = malloc(strlen(in_val));
    assert(key && val);
    key_list_push(keys, &key);
    val_list_push(vals, &val);
}

void*
__wrap_libuboot_iterator(struct uboot_ctx* ctx, void* next)
{
    static bool result = false;
    if (g_next) {
        char *key = NULL, *val = NULL;
        key = key_list_pop(keys);
        val = val_list_pop(vals);
        if (key) free(key);
        if (val) free(val);
    } else {
        g_next = true;
    }
    result = key_list_size(keys) ? true : false;
    return &result;
}

const char*
__wrap_libuboot_getname(void* entry)
{
    return key_list_front(keys);
}

const char*
__wrap_libuboot_getvalue(void* entry)
{
    return val_list_front(vals);
}
