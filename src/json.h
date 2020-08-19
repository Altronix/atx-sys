#ifndef JSON_H
#define JSON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define JSMN_HEADER
#include "jsmn.h"

typedef struct jsmn_value
{
    const char* p;
    uint32_t len;
} jsmn_value;

#define json_for_each_obj(i, t, obj)                                           \
    for (i = 0, t = (obj) + 1; i < (obj)->size; t = json_next(t + 1), i++)
#define json_for_each_arr(i, t, arr)                                           \
    for (i = 0, t = (arr) + 1; i < (arr)->size; t = json_next(t), i++)

bool json_tok_is_null(const char* buffer, const jsmntok_t* tok);
const jsmntok_t* json_next(const jsmntok_t* tok);
const jsmntok_t*
json_get_member(const char* buffer, const jsmntok_t tok[], const char* label);
const jsmntok_t* json_get_arr(const jsmntok_t tok[], size_t index);
bool json_tok_streq(const char* buffer, const jsmntok_t* tok, const char* str);
jsmn_value json_tok_value(const char* b, const jsmntok_t* t);
jsmn_value
json_delve_value(const char* buff, jsmntok_t* tok, const char* guide);
const jsmntok_t*
json_delve(const char* buf, const jsmntok_t* tok, const char* guide);
int json_to_u64(const char* buffer, const jsmntok_t* tok, uint64_t* ret);
int json_to_s64(const char* buffer, const jsmntok_t* tok, int64_t* ret);
int json_to_u32(const char* buffer, const jsmntok_t* tok, uint32_t* ret);
int json_to_s32(const char* buffer, const jsmntok_t* tok, int32_t* ret);
int json_to_u16(const char* buffer, const jsmntok_t* tok, uint16_t* ret);
int json_to_s16(const char* buffer, const jsmntok_t* tok, int16_t* ret);
int json_to_u8(const char* buffer, const jsmntok_t* tok, uint8_t* ret);
int json_to_s8(const char* buffer, const jsmntok_t* tok, int8_t* ret);

#endif
