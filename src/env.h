#ifndef ENV_H
#define ENV_H

#include "private.h"

#if ATX_UPDATE_ENV == UBOOT
#include "libuboot.h"
typedef struct uboot_ctx env_ctx;
#elif ATX_UPDATE_ENV == EMU
typedef void env_ctx;
#endif

typedef struct env_s
{
    env_ctx* ctx;
} env_s;

int env_init(env_s* env);
void env_close(env_s* env);
int env_read(env_s* env, char* buff, uint32_t* b);
int env_read_var(env_s* env, const char* var, uint32_t l);
int env_write(env_s* env);

#endif /* ENV_H */
