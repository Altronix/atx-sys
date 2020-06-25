#ifndef ATXUPDATE_H
#define ATXUPDATE_H

#include "stdbool.h"
#include "stdint.h"

typedef struct atxupdate_config_s
{
    const char* env;
    const char* port;
    bool daemon;
} atxupdate_config_s;

typedef struct atxupdate_s atxupdate_s;

atxupdate_s* atxupdate_create(atxupdate_config_s* config);
void atxupdate_destroy(atxupdate_s**);

#endif /* ATXUPDATE_H */
