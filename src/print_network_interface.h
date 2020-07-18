#ifndef PRINT_NETWORK_INTERFACE_H
#define PRINT_NETWORK_INTERFACE_H

#include "private.h"

int print_network_interface(
    FILE* f,
    const char* meth,
    const char* ip,
    const char* sn,
    const char* gw,
    const char* hn);

#endif /* PRINT_NETWORK_INTERFACE_H */
