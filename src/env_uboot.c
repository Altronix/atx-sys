/* sys */
#include "config.h"
#include "private.h"

/* app */
#include "env.h"
#include "log.h"

int
env_init(env_s* env)
{
    memset(env, 0, sizeof(env_s));

    // Initialize uboot library
    int ret = libuboot_initialize(&env->ctx, NULL);
    if (ret < 0) {
        log_error("libuboot failed to initialize [%d]", ret);
        goto ERR;
    }

    // Read uboot env config from the system
    ret = libuboot_read_config(env->ctx, ATX_UPDATE_ENV_CONFIG_FILE);
    if (ret < 0) {
        libuboot_exit(env->ctx);
        log_error("libuboot failed to read config! [%d]", ret);
        log_error("(file: [%s])", ATX_UPDATE_ENV_CONFIG_FILE);
        goto ERR;
    }

    // Load the environment from partition
    ret = libuboot_open(env->ctx);
    if (ret < 0) {
        libuboot_exit(env->ctx);
        log_error("libuboot failed to load environment! [%d]", ret);
        goto ERR;
    }

    log_info("libubootenv load ok!");
    return ret;
ERR:
    return ret;
}

void
env_close(env_s* env)
{
    libuboot_close(env->ctx);
    libuboot_exit(env->ctx);
    memset(env, 0, sizeof(env_s));
}

int
env_read(env_s* env, char* buff, uint32_t* b)
{
    void* iter = NULL;
    const char *name, *value;
    uint32_t len = *b;
    *b = 1;
    if (len) *buff = '{';

    iter = libuboot_iterator(env->ctx, iter);
    while (iter) {
        name = libuboot_getname(iter);
        value = libuboot_getvalue(iter);
        *b += snprintf(&buff[*b], len - *b, "\"%s\":\"%s\"", name, value);
        if ((iter = libuboot_iterator(env->ctx, iter))) {
            if (*b < len) buff[(*b)++] = ',';
        }
    }
    if (*b < len) buff[(*b)++] = '}';
    if (*b < len) buff[*b] = 0;
    return *b < len ? 0 : -1;
}

int
env_read_var(env_s* env, const char* var, uint32_t l)
{
    return -1;
}

int
env_write(env_s* env)
{
    return -1;
}
