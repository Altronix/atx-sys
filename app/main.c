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
print_usage_and_exit(int code)
{
    fprintf(stdout, "%s", usage);
    exit(code);
}

static void
args_parse(atxupdate_config_s* config, int argc, char* argv[])
{
    int opt, count = 0;
    memset(config, 0, sizeof(atxupdate_config_s));
    optind = 0;
    while ((opt = getopt(argc, argv, "pchd?")) != -1) {
        count++;
        switch (opt) {
            case 'p': config->port = argv[optind]; break;
            case 'c': config->env = argv[optind]; break;
            case 'd': config->daemon = true; break;
            case '?':
            case 'h': print_usage_and_exit(0); break;
            default: print_usage_and_exit(-1); break;
        }
    }
    if (!count) print_usage_and_exit(0);
}

int
main(int argc, char* argv[])
{
    atxupdate_config_s config;
    // TODO load defaults
    args_parse(&config, argc, argv);
    signal(SIGINT, ctrlc);

    atxupdate_s* atxupdate = atxupdate_create(&config);

    while (running) atxupdate_poll(atxupdate, 50);

    atxupdate_destroy(&atxupdate);
    return 0;
}
