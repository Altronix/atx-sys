#ifndef ATXUPDATE_H
#define ATXUPDATE_H

#include "stdbool.h"
#include "stdint.h"

#ifndef ATX_SYS_ENV_CONFIG_FILE
#define ATX_SYS_ENV_CONFIG_FILE "/etc/fw_env.config"
#endif

#ifndef ATX_SYS_CONFIG_FILE
#define ATX_SYS_CONFIG_FILE "/etc/atx.config.sys.json"
#endif

#ifndef ATX_SYS_HTTP_PORT
#define ATX_SYS_HTTP_PORT "8080"
#endif

#ifndef ATX_SYS_HOSTNAME_DEFAULT
#define ATX_SYS_HOSTNAME_DEFAULT "LinQM5"
#endif

#ifndef ATX_SYS_METH_DEFAULT
#define ATX_SYS_METH_DEFAULT "DHCP"
#endif

#ifndef ATX_SYS_IP_DEFAULT
#define ATX_SYS_IP_DEFAULT "192.168.168.168"
#endif

#ifndef ATX_SYS_SN_DEFAULT
#define ATX_SYS_SN_DEFAULT "255.255.255.0"
#endif

#ifndef ATX_SYS_GW_DEFAULT
#define ATX_SYS_GW_DEFAULT "192.168.168.1"
#endif

typedef struct atxupdate_config_s
{
    const char* log;
    const char* env;
    const char* usr;
    const char* port;
    const char* www;
    bool daemon;
} atxupdate_config_s;

typedef struct atxupdate_s atxupdate_s;

atxupdate_s* atxupdate_create(atxupdate_config_s* config);
void atxupdate_destroy(atxupdate_s**);
void atxupdate_poll(atxupdate_s*, uint32_t);
bool atxupdate_is_running(atxupdate_s* u);

#endif /* ATXUPDATE_H */
