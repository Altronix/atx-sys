#include "private.h"

#include "altronix/atxsys.h"
#include "env.h"
#include "http.h"
#include "log.h"
#include "network_management.h"

typedef struct atxsys_s
{
    env_s env;
    http_s http;
    pid_t pid;
    FILE* log;
} atxsys_s;

static int
read_user_config(
    atxsys_config_s* c,
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
    int err = -1;
    static const char *meth_default = ATX_SYS_METH_DEFAULT,
                      *hn_default = ATX_SYS_HOSTNAME_DEFAULT,
                      *ip_default = ATX_SYS_IP_DEFAULT,
                      *sn_default = ATX_SYS_SN_DEFAULT,
                      *gw_default = ATX_SYS_GW_DEFAULT;

    // Invalid config file location
    if (!(c->usr && *c->usr)) goto ERR;

    err = stat(c->usr, &st);
    if (!err && st.st_size < l && (f = fopen(c->usr, "r"))) {
        log_info("(SYS) config file found [%s]", c->usr);
        err = fread(b, 1, st.st_size, f);
        if (!(err == st.st_size)) goto ERR;
        err = parse_network_config(b, st.st_size, meth, ip, sn, gw, hn);
        if (err) goto ERR;
        fclose(f);
    } else if (err && errno == ENOENT && (f = fopen(c->usr, "w+"))) {
        log_info("(SYS) config file not found! ...creating [%s]", c->usr);
        (meth->p = meth_default, meth->len = strlen(meth->p));
        (hn->p = hn_default, hn->len = strlen(hn->p));
        (ip->p = ip_default, ip->len = strlen(ip->p));
        (sn->p = sn_default, sn->len = strlen(sn->p));
        (gw->p = gw_default, gw->len = strlen(gw->p));
        err = print_network_config(f, meth, ip, sn, gw, hn);
        err = err <= 0 ? -1 : 0;
        fclose(f);
    }

    return err;
ERR:
    fclose(f);
    return -1;
}

atxsys_s*
atxsys_create(atxsys_config_s* c)
{
    int err, spot = 0;
    char b[2048] = { 0 };
    pid_t pid;
    memset(&pid, 0, sizeof(pid));
    jsmn_value meth, ip, sn, gw, hn;
    FILE* f;

    // Read user config, if valid config, write network interface
    if (!read_user_config(c, b, sizeof(b), &meth, &ip, &sn, &gw, &hn) &&
        (f = fopen("/etc/network/interfaces", "w+"))) {
        print_network_interface(f, &meth, &ip, &sn, &gw, &hn);
        fclose(f);
    } else {
        log_error("(SYS) Failed to write /etc/network/interfaces");
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
    atxsys_s* u = malloc(sizeof(atxsys_s));
    if (u) {
        memset(u, 0, sizeof(atxsys_s));
        const char* port = c->port ? c->port : ATX_SYS_HTTP_PORT;
        const char* env = c->env ? c->env : ATX_SYS_ENV_CONFIG_FILE;
        if (*b && (u->log = fopen(b, "a+"))) {
            log_set_fd(&u->log);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }
        env_init(&u->env, env);
        http_init(&u->http, &u->env, env);
        if (c->www) http_serve(&u->http, c->www);
        http_listen(&u->http, port);
    }
    return u;
}

void
atxsys_destroy(atxsys_s** update_p)
{
    atxsys_s* u = *update_p;
    *update_p = NULL;
    http_deinit(&u->http);
    env_deinit(&u->env);
    if (u->log) fclose(u->log);
    memset(u, 0, sizeof(atxsys_s));
    free(u);
}

void
atxsys_poll(atxsys_s* u, uint32_t ms)
{
    http_poll(&u->http, ms);
}

bool
atxsys_is_running(atxsys_s* u)
{
    return !u->http.shutdown;
}
