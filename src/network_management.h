#ifndef NETWORK_MANAGEMENT_H
#define NETWORK_MANAGEMENT_H

#include "json.h"
#include "private.h"

// Write a network/interfaces file
int print_network_interface(
    FILE* f,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hn);

// Parse a json config buffer
int parse_network_config(
    const char* buff,
    uint32_t len,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hostname);

// Write a json config file
int print_network_config(
    FILE* f,
    jsmn_value* meth,
    jsmn_value* ip,
    jsmn_value* sn,
    jsmn_value* gw,
    jsmn_value* hn,
    jsmn_value* pri,
    jsmn_value* sec);
int print_network_config_toks(
    FILE* f,
    const char* b,
    const jsmntok_t* meth,
    const jsmntok_t* ip,
    const jsmntok_t* sn,
    const jsmntok_t* gw,
    const jsmntok_t* hn,
    const jsmntok_t* pri,
    const jsmntok_t* sec);

#endif /* NETWORK_MANAGEMENT_H */
