/* sys */
#include "private.h"

/* app */
#include "env.h"
#include "log.h"

int
env_init(env_s* env, const char* path)
{
    memset(env, 0, sizeof(env_s));

    // Initialize uboot library
    int ret = libuboot_initialize(&env->ctx, NULL);
    if (ret < 0) {
        log_error("libuboot failed to initialize [%d]", ret);
        goto ERR;
    }

    // Read uboot env config from the system
    ret = libuboot_read_config(env->ctx, path);
    if (ret < 0) {
        libuboot_exit(env->ctx);
        log_error("libuboot failed to read config! [%d]", ret);
        log_error("(file: [%s])", path);
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
    env->valid = true;
    return ret;
ERR:
    return ret;
}

void
env_deinit(env_s* env)
{
    if (env->valid) {
        libuboot_close(env->ctx);
        libuboot_exit(env->ctx);
    }
    memset(env, 0, sizeof(env_s));
}

int
env_read(env_s* env, char* buff, uint32_t* b)
{
    void* iter = NULL;
    const char *name, *value;
    uint32_t len = *b;
    char temp[128];
    *b = 1;
    if (len) *buff = '{';

    iter = libuboot_iterator(env->ctx, iter);
    while (iter) {
        name = libuboot_getname(iter);
        value = libuboot_getvalue(iter);
        snprintf(temp, sizeof(temp), "%s", value);
        for (int i = 0, templen = strlen(temp); i < templen; i++) {
            if (temp[i] == '"') temp[i] = '\'';
        }
        *b += snprintf(&buff[*b], len - *b, "\"%s\":\"%s\"", name, temp);
        if ((iter = libuboot_iterator(env->ctx, iter))) {
            if (*b < len) buff[(*b)++] = ',';
        }
    }
    if (*b < len) buff[(*b)++] = '}';
    if (*b < len) buff[*b] = 0;
    return *b < len ? 0 : -1;
}

const char*
env_read_val(env_s* env, const char* key)
{
    const char *name, *ret = NULL;
    void* iter = NULL;
    while ((iter = libuboot_iterator(env->ctx, iter))) {
        name = libuboot_getname(iter);
        if (!strcmp(name, key)) {
            ret = 0;
            ret = libuboot_getvalue(iter);
            break;
        }
    }
    return ret;
}

int
env_write(env_s* env)
{
    return -1;
}
