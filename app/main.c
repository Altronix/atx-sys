#include "altronix/atxupdate.h"

#include "stdarg.h"
#include "stdbool.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile int running = 1;

void
ctrlc(int dummy)
{
    running = 0;
}

const char* usage =
    "Usage: atx-update [-pchd]\n"
    "  -p  HTTP port (default: 8080)\n"
    "  -c  Bootloader Environment (default: /etc/fw_env.config)\n"
    "  -d  Detatch in daemon mode\n"
    "  -h  Print this help menu\n";

static void
args_parse(atxupdate_config_s* config, int argc, char* argv[])
{
    int opt, arglen;
    memset(config, 0, sizeof(atxupdate_config_s));
    optind = 0;
    while ((opt = getopt(argc, argv, "pchd?")) != -1) {
        switch (opt) {
            case 'p': config->port = argv[optind]; break;
            case 'c': config->env = argv[optind]; break;
            case 'd': config->daemon = true; break;
            case '?':
            case 'h': fprintf(stdout, "%s", usage); break;
            default:
                fprintf(stderr, "Usage: %s [-pch?]", argv[0]);
                exit(-1);
                break;
        }
    }
}

int
main(int argc, char* argv[])
{
    atxupdate_config_s config;
    args_parse(&config, argc, argv);
    signal(SIGINT, ctrlc);

    atxupdate_s* atxupdate = atxupdate_create(&config);

    while (running) atxupdate_poll(atxupdate, 50);

    atxupdate_destroy(&atxupdate);
    return 0;
}
