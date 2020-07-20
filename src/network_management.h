#ifndef NETWORK_MANAGEMENT_H
#define NETWORK_MANAGEMENT_H

#include "json.h"
#include "private.h"

int print_network_interface(
    FILE* f,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hn);
int parse_network_interface(
    const char* buff,
    uint32_t len,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hostname);

#endif /* NETWORK_MANAGEMENT_H */
