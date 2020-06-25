#include "private.h"

#include "altronix/atxupdate.h"
#include "config.h"
#include "env.h"
#include "http.h"

typedef struct atxupdate_s
{
    env_s env;
    http_s http;
} atxupdate_s;

atxupdate_s*
atxupdate_create(atxupdate_config_s* c)
{
    atxupdate_s* u = malloc(sizeof(atxupdate_s));
    if (u) {
        const char* port = c->port ? c->port : ATX_UPDATE_HTTP_PORT;
        const char* env = c->env ? c->env : ATX_UPDATE_ENV_CONFIG_FILE;
        env_init(&u->env, env);
        http_init(&u->http, &u->env);
        http_listen(&u->http, port);
    }
    return u;
}

void
atxupdate_destroy(atxupdate_s** update_p)
{
    atxupdate_s* u = *update_p;
    *update_p = NULL;
    http_deinit(&u->http);
    env_deinit(&u->env);
    memset(u, 0, sizeof(atxupdate_s));
    free(u);
}
