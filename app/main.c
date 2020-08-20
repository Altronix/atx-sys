#include "altronix/atxsys.h"

#include "stdarg.h"
#include "stdbool.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static volatile int running = 1;

void
ctrlc(int dummy)
{
    running = 0;
}

void
sighup(int dummy)
{}

const char* usage =
    "Usage: atx-update [-pchdl]\n"
    "  -p  HTTP port (default: " ATX_SYS_HTTP_PORT ")\n"
    "  -e  Bootloader Environment (default: " ATX_SYS_ENV_CONFIG_FILE ")\n"
    "  -u  User configuration (default: " ATX_SYS_CONFIG_FILE ")\n"
    "  -d  Detatch in daemon mode\n"
    "  -l  Log file (default: " ATX_SYS_LOG_FILE_DEFAULT ")\n"
    "  -w  Serve webpage root dir\n"
    "  -h  Print this help menu\n";

static void
print_usage_and_exit(int code)
{
    fprintf(stdout, "%s", usage);
    exit(code);
}

static void
args_parse(atxsys_config_s* config, int argc, char* argv[])
{
    int opt, count = 0;
    optind = 0;
    while ((opt = getopt(argc, argv, "lwpehdu?")) != -1) {
        count++;
        switch (opt) {
            case 'p': config->port = argv[optind]; break;
            case 'e': config->env = argv[optind]; break;
            case 'u': config->usr = argv[optind]; break;
            case 'd': config->daemon = true; break;
            case 'l': config->log = argv[optind]; break;
            case 'w': config->www = argv[optind]; break;
            case '?':
            case 'h': print_usage_and_exit(0); break;
            default: print_usage_and_exit(-1); break;
        }
    }
}

int
main(int argc, char* argv[])
{
    int err;
    pid_t pid;
    atxsys_config_s config;
    memset(&config, 0, sizeof(atxsys_config_s));
    config.port = ATX_SYS_HTTP_PORT;
    config.env = ATX_SYS_ENV_CONFIG_FILE;
    config.usr = ATX_SYS_CONFIG_FILE;
    args_parse(&config, argc, argv);

    signal(SIGINT, ctrlc);
    signal(SIGHUP, sighup);

    atxsys_s* atxsys = atxsys_create(&config);

    while (running && atxsys_is_running(atxsys)) { atxsys_poll(atxsys, 50); }

    atxsys_destroy(&atxsys);
    return 0;
}
