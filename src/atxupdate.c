#include "private.h"

#include "altronix/atxupdate.h"
#include "env.h"
#include "http.h"
#include "log.h"
#include "network_management.h"

typedef struct atxupdate_s
{
    env_s env;
    http_s http;
    pid_t pid;
    FILE* log;
} atxupdate_s;

static int
read_user_config(
    atxupdate_config_s* c,
    char* b,
    uint32_t l,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hn)
{
    FILE* f;
    struct stat st;
    int err;

    // If config file exists and can fit in buffer
    if (c->usr && *c->usr && !stat(c->usr, &st) && st.st_size < l &&
        (f = fopen(c->usr, "r"))) {
        err = fread(b, 1, st.st_size, f);
        if (!(err == st.st_size)) goto ERR;
        err = parse_network_interface(b, st.st_size, meth, ip, sn, gw, hn);
        if (err) goto ERR;
        fclose(f);
    }
    return 0;
ERR:
    fclose(f);
    return -1;
}

static int
write_network_interface(
    atxupdate_config_s* c,
    const char* path,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hn)
{
    int err = -1;
    FILE* f;
    if ((f = fopen(path, "w+"))) {
        err = print_network_interface(f, meth, ip, sn, gw, hn);
    }
    return err;
}

atxupdate_s*
atxupdate_create(atxupdate_config_s* c)
{
    int err, spot = 0;
    char b[2048] = { 0 };
    pid_t pid;
    memset(&pid, 0, sizeof(pid));
    jsmn_value meth, ip, sn, gw, hn;
    const char* iface = "/etc/network/interface";

    // Read user config, if valid config, write network interface
    if (!read_user_config(c, b, sizeof(b), &meth, &ip, &sn, &gw, &hn)) {
        write_network_interface(c, iface, &meth, &ip, &sn, &gw, &hn);
    }

    // Normalize log path (make absolute relative to start path)
    *b = '\0';
    if (c->log) {
        if (!(*c->log == '/' || *c->log == '\\')) {
            getcwd(b, sizeof(b));
            spot = strlen(b);
            if (spot < sizeof(b) - spot) b[spot++] = '/';
        }
        spot += snprintf(&b[spot], sizeof(b) - spot, "%s", c->log);
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
        const char* port = c->port ? c->port : ATX_SYS_HTTP_PORT;
        const char* env = c->env ? c->env : ATX_SYS_ENV_CONFIG_FILE;
        if (*b && (u->log = fopen(b, "a+"))) {
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
