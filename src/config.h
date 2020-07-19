#ifndef CONFIG_H
#define CONFIG_H

#ifndef ATX_SYS_ENV_CONFIG_FILE
#define ATX_SYS_ENV_CONFIG_FILE "/etc/fw_env.config"
#endif

#ifndef ATX_SYS_USER_CONFIG_FILE
#define ATX_SYS_USER_CONFIG_FILE "/etc/atxsys.config.json"
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

#endif /* CONFIG_H */
