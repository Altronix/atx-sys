// Copyright (c) 2019-2020 Altronix Corp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include "klist.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define GENERIC_FREE_FN(type)                                                  \
    void type##_free_fn(type** ctx_p)                                          \
    {                                                                          \
        type* ctx = *ctx_p;                                                    \
        *ctx_p = NULL;                                                         \
        free(ctx);                                                             \
    }

#define EMPTY_FREE_FN(type)                                                    \
    void type##_free_fn(type** ctx_p) {}

    static inline void __list_free_fn(void* ctx) { ((void)ctx); }
#define FREE_FN(x) __list_free_fn(x->data)

#define LIST_INIT_H(tag, type)                                                 \
    typedef kl_##tag##_t tag##_list_s;                                         \
    typedef kl1_##tag tag##_item_s;                                            \
    tag##_list_s* tag##_list_create();                                         \
    void tag##_list_destroy(tag##_list_s** list_p);                            \
    type* tag##_list_front(tag##_list_s* list);                                \
    void tag##_list_push(tag##_list_s* list, type** r_p);                      \
    type* tag##_list_pop(tag##_list_s* l);                                     \
    uint32_t tag##_list_size(tag##_list_s* l);

#define LIST_INIT_NO_FREE(tag, type)                                           \
    EMPTY_FREE_FN(type)                                                        \
    LIST_INIT(tag, type, type##_free_fn)

#define LIST_INIT_W_FREE(tag, type)                                            \
    GENERIC_FREE_FN(type)                                                      \
    LIST_INIT(tag, type, type##_free_fn)

#define LIST_INIT(tag, type, list_free_fn)                                     \
    KLIST_INIT(tag, type*, FREE_FN)                                            \
                                                                               \
    kl_##tag##_t* tag##_list_create();                                         \
    void tag##_list_destroy(kl_##tag##_t**);                                   \
    void tag##_list_push(kl_##tag##_t*, type**);                               \
    type* tag##_list_pop(kl_##tag##_t*);                                       \
                                                                               \
    typedef kl_##tag##_t tag##_list_s;                                         \
    typedef kl1_##tag tag##_item_s;                                            \
                                                                               \
    tag##_list_s* tag##_list_create() { return kl_init_##tag(); }              \
                                                                               \
    void tag##_list_destroy(tag##_list_s** list_p)                             \
    {                                                                          \
        tag##_list_s* list = *list_p;                                          \
        *list_p = NULL;                                                        \
        type* next = tag##_list_pop(list);                                     \
        while (next) {                                                         \
            type* deleteme = next;                                             \
            next = tag##_list_pop(list);                                       \
            list_free_fn(&deleteme);                                           \
        }                                                                      \
        kl_destroy_##tag(list);                                                \
    }                                                                          \
                                                                               \
    type* tag##_list_front(tag##_list_s* list) { return list->head->data; }    \
                                                                               \
    void tag##_list_push(tag##_list_s* list, type** r_p)                       \
    {                                                                          \
        type* r = *r_p;                                                        \
        *r_p = NULL;                                                           \
        *kl_pushp_##tag(list) = r;                                             \
    }                                                                          \
                                                                               \
    type* tag##_list_pop(tag##_list_s* l)                                      \
    {                                                                          \
        type* ret = NULL;                                                      \
        kl_shift_##tag(l, &ret);                                               \
        return ret;                                                            \
    }                                                                          \
                                                                               \
    uint32_t tag##_list_size(tag##_list_s* l) { return l->size; }

#define MAP_INIT_H(tag, type)                                                  \
    typedef struct kh_##tag##_s tag##_map_s;                                   \
    typedef void (*tag##_map_foreach_fn)(                                      \
        tag##_map_s*, void*, const char*, type**);                             \
    tag##_map_s* tag##_map_create();                                           \
    void tag##_map_destroy(tag##_map_s** map_p);                               \
    type** tag##_map_add(tag##_map_s* nodes, const char* key, type** node_p);  \
    void tag##_map_remove_iter(tag##_map_s* nodes, khiter_t k);                \
    void tag##_map_remove(tag##_map_s* nodes, const char* serial);             \
    type** tag##_map_get(tag##_map_s* hash, const char* serial);               \
    type** tag##_map_resolve(tag##_map_s* hash, khiter_t k);                   \
    khiter_t tag##_map_key(tag##_map_s* hash, const char* serial);             \
    uint32_t tag##_map_size(tag##_map_s* hash);                                \
    void tag##_map_foreach(tag##_map_s*, tag##_map_foreach_fn, void*);

#define MAP_INIT_NO_FREE(tag, type)                                            \
    EMPTY_FREE_FN(type)                                                        \
    MAP_INIT(tag, type, type##_free_fn)

#define MAP_INIT_W_FREE(tag, type)                                             \
    GENERIC_FREE_FN(type)                                                      \
    MAP_INIT(tag, type, type##_free_fn)

#define MAP_INIT(tag, type, map_free_fn)                                       \
    KHASH_MAP_INIT_STR(tag, type*)                                             \
                                                                               \
    typedef struct kh_##tag##_s tag##_map_s;                                   \
    typedef void (*tag##_map_foreach_fn)(                                      \
        tag##_map_s*, void*, const char* key, type**);                         \
                                                                               \
    tag##_map_s* tag##_map_create() { return kh_init_##tag(); }                \
                                                                               \
    void tag##_map_destroy(tag##_map_s** map_p)                                \
    {                                                                          \
        tag##_map_s* hash = *map_p;                                            \
        *map_p = NULL;                                                         \
        for (khint_t k = kh_begin(hash); k != kh_end(hash); ++k) {             \
            if (kh_exist(hash, k)) {                                           \
                type* d = kh_val(hash, k);                                     \
                map_free_fn(&d);                                               \
            }                                                                  \
        }                                                                      \
        kh_destroy_##tag(hash);                                                \
    }                                                                          \
                                                                               \
    type** tag##_map_add(tag##_map_s* nodes, const char* key, type** node_p)   \
    {                                                                          \
        int ret = 0;                                                           \
        type* node = *node_p;                                                  \
        *node_p = NULL;                                                        \
        khiter_t k = kh_put_##tag(nodes, key, &ret);                           \
        assert(ret >= 0);                                                      \
        kh_val(nodes, k) = node;                                               \
        return &kh_val(nodes, k);                                              \
    }                                                                          \
                                                                               \
    void tag##_map_remove_iter(tag##_map_s* nodes, khiter_t k)                 \
    {                                                                          \
        type* d = kh_val(nodes, k);                                            \
        kh_del_##tag(nodes, k);                                                \
        map_free_fn(&d);                                                       \
    }                                                                          \
                                                                               \
    void tag##_map_remove(tag##_map_s* nodes, const char* serial)              \
    {                                                                          \
        khiter_t k;                                                            \
        if (!((k = kh_get_##tag(nodes, serial)) == kh_end(nodes))) {           \
            tag##_map_remove_iter(nodes, k);                                   \
        }                                                                      \
    }                                                                          \
                                                                               \
    type** tag##_map_get(tag##_map_s* hash, const char* serial)                \
    {                                                                          \
        khiter_t k;                                                            \
        return ((k = kh_get_##tag(hash, serial)) == kh_end(hash))              \
                   ? NULL                                                      \
                   : &kh_val(hash, k);                                         \
    }                                                                          \
                                                                               \
    type** tag##_map_resolve(tag##_map_s* hash, khiter_t k)                    \
    {                                                                          \
        return &kh_val(hash, k);                                               \
    }                                                                          \
                                                                               \
    khiter_t tag##_map_key(tag##_map_s* hash, const char* serial)              \
    {                                                                          \
        khiter_t k;                                                            \
        return ((k = kh_get_##tag(hash, serial)) == kh_end(hash)) ? 0 : k;     \
    }                                                                          \
                                                                               \
    uint32_t tag##_map_size(tag##_map_s* hash) { return kh_size(hash); }       \
                                                                               \
    void tag##_map_foreach(                                                    \
        tag##_map_s* hash, tag##_map_foreach_fn fn, void* ctx)                 \
    {                                                                          \
        for (khiter_t k = kh_begin(hash); k != kh_end(hash); ++k) {            \
            if (kh_exist(hash, k))                                             \
                fn(hash, ctx, kh_key(hash, k), &kh_val(hash, k));              \
        }                                                                      \
    }

#ifdef __cplusplus
}
#endif
#endif

