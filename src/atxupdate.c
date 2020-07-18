#include "private.h"

#include "altronix/atxupdate.h"
#include "config.h"
#include "env.h"
#include "http.h"
#include "log.h"
#include "print_network_interface.h"

typedef struct atxupdate_s
{
    env_s env;
    http_s http;
    pid_t pid;
    FILE* log;
} atxupdate_s;

atxupdate_s*
atxupdate_create(atxupdate_config_s* c)
{
    int err, spot = 0;
    char logpath[2048] = { 0 };
    pid_t pid;
    memset(&pid, 0, sizeof(pid));

    // Normalize log path (make absolute relative to start path)
    if (c->log) {
        if (!(*c->log == '/' || *c->log == '\\')) {
            getcwd(logpath, sizeof(logpath));
            spot = strlen(logpath);
            if (spot < sizeof(logpath) - spot) logpath[spot++] = '/';
        }
        spot += snprintf(&logpath[spot], sizeof(logpath) - spot, "%s", c->log);
    }

    if (c->daemon) {
        err = fork();
        if (err < 0) exit(EXIT_FAILURE);
        if (err > 0) exit(EXIT_SUCCESS);
        umask(0);
        pid = setsid();
        if (pid < 0) exit(EXIT_FAILURE);
        err = chdir("/");
        if (err < 0) exit(EXIT_FAILURE);
    }
    atxupdate_s* u = malloc(sizeof(atxupdate_s));
    if (u) {
        memset(u, 0, sizeof(atxupdate_s));
        const char* port = c->port ? c->port : ATX_UPDATE_HTTP_PORT;
        const char* env = c->env ? c->env : ATX_UPDATE_ENV_CONFIG_FILE;
        if (*logpath && (u->log = fopen(logpath, "a+"))) {
            log_set_fd(&u->log);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }
        env_init(&u->env, env);
        http_init(&u->http, &u->env);
        if (c->www) http_serve(&u->http, c->www);
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
    if (u->log) fclose(u->log);
    memset(u, 0, sizeof(atxupdate_s));
    free(u);
}

void
atxupdate_poll(atxupdate_s* u, uint32_t ms)
{
    http_poll(&u->http, ms);
}

bool
atxupdate_is_running(atxupdate_s* u)
{
    return !u->http.shutdown;
}
