#define JSMN_STRICT 1
#include "jsmn.h"

#include "json.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

bool
json_tok_is_null(const char* buffer, const jsmntok_t* tok)
{
    if (tok->type != JSMN_PRIMITIVE) return false;
    return buffer[tok->start] == 'n';
}

const jsmntok_t*
json_next(const jsmntok_t* tok)
{
    const jsmntok_t* t;
    size_t i;

    for (t = tok + 1, i = 0; i < tok->size; i++) t = json_next(t);

    return t;
}

const jsmntok_t*
json_get_member(const char* buffer, const jsmntok_t tok[], const char* label)
{
    const jsmntok_t* t;
    size_t i;

    if (tok->type != JSMN_OBJECT) return NULL;

    json_for_each_obj(
        i, t, tok) if (json_tok_streq(buffer, t, label)) return t +
        1;

    return NULL;
}

const jsmntok_t*
json_get_arr(const jsmntok_t tok[], size_t index)
{
    const jsmntok_t* t;
    size_t i;

    if (tok->type != JSMN_ARRAY) return NULL;

    json_for_each_arr(i, t, tok)
    {
        if (index == 0) return t;
        index--;
    }

    return NULL;
}

bool
json_tok_streq(const char* buffer, const jsmntok_t* tok, const char* str)
{
    if (tok->type != JSMN_STRING) return false;
    if (tok->end - tok->start != strlen(str)) return false;
    return strncmp(buffer + tok->start, str, tok->end - tok->start) == 0;
}

jsmn_value
json_tok_value(const char* b, const jsmntok_t* t)
{
    jsmn_value ret;
    ret.p = &b[t->start];
    ret.len = t->end - t->start;
    return ret;
}

jsmn_value
json_delve_value(const char* buff, jsmntok_t* tok, const char* guide)
{
    jsmn_value ret = { .p = NULL, .len = 0 };
    const jsmntok_t* t = json_delve(buff, tok, guide);
    return t ? json_tok_value(buff, t) : ret;
}

const jsmntok_t*
json_delve(const char* buf, const jsmntok_t* tok, const char* guide)
{
    char key[128];
    while (*guide) {
        int len, sz = strcspn(guide + 1, ".");
        len = snprintf(key, sizeof(key), "%.*s", sz, guide + 1);
        assert(len < sizeof(key));
        switch (guide[0]) {
            case '.':
                if (!(tok->type == JSMN_OBJECT)) return NULL;
                tok = json_get_member(buf, tok, key);
                if (!tok) return NULL;
                break;
            default: abort();
        }
        guide += sz + 1;
    }
    return tok;
}
int
json_to_u64(const char* buf, const jsmntok_t* tok, uint64_t* n)
{
    char* end;
    unsigned long long l;
    l = strtoull(buf + tok->start, &end, 0);
    if (end != buf + tok->end) return false;
    assert(sizeof(l) >= sizeof(*n));
    *n = l;
    if ((l == ULLONG_MAX) && errno == ERANGE) return -1;
    return *n == l ? 0 : -1;
}

int
json_to_s64(const char* buf, const jsmntok_t* tok, int64_t* n)
{
    char* end;
    unsigned long long l;
    l = strtoull(buf + tok->start, &end, 0);
    if (end != buf + tok->end) return false;
    assert(sizeof(l) >= sizeof(*n));
    *n = l;
    if ((l == LONG_MAX || l == LONG_MIN) && errno == ERANGE) return -1;
    return *n == l ? 0 : -1;
}
#define make_json_to_u(id, type)                                               \
    int json_to_##id(const char* buffer, const jsmntok_t* tok, type* ret)      \
    {                                                                          \
        uint64_t x;                                                            \
        if (json_to_u64(buffer, tok, &x)) return -1;                           \
        *ret = x;                                                              \
        return *ret == x ? 0 : -1;                                             \
    }

#define make_json_to_s(id, type)                                               \
    int json_to_##id(const char* buffer, const jsmntok_t* tok, type* ret)      \
    {                                                                          \
        int64_t x;                                                             \
        if (json_to_s64(buffer, tok, &x)) return -1;                           \
        *ret = x;                                                              \
        return *ret == x ? 0 : -1;                                             \
    }
make_json_to_u(u32, uint32_t);
make_json_to_s(s32, int32_t);
make_json_to_u(u16, uint16_t);
make_json_to_s(s16, int16_t);
make_json_to_u(u8, uint8_t);
make_json_to_s(s8, int8_t);
