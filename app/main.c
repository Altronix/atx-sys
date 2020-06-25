#include "altronix/atxupdate.h"

#include "stdarg.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* usage = "Usage: atx-update [-pchd?]\n"
                    "-p  HTTP port\n"
                    "-c  Bootloader Environment (ie /etc/fw_env.config)\n"
                    "-h  Print this help menu\n"
                    "-d  Detatch in daemon mode\n"
                    "-?  Print this help menu\n";

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
    return 0;
}
