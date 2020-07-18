#ifndef ENV_H
#define ENV_H

#include "private.h"

#if EMU == FALSE
#include "libuboot.h"
typedef struct uboot_ctx env_ctx;
#else
typedef void env_ctx;
#endif

typedef struct env_s
{
    env_ctx* ctx;
    bool valid;
} env_s;

int env_init(env_s* env, const char* path);
void env_deinit(env_s* env);
int env_read(env_s* env, char* buff, uint32_t* b);
const char* env_read_val(env_s* env, const char* key);
int env_write(env_s* env);

#endif /* ENV_H */
