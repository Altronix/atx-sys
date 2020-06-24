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

#endif /* ENV_H */
