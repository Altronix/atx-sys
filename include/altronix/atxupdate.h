#ifndef ATXUPDATE_H
#define ATXUPDATE_H

#include "stdbool.h"
#include "stdint.h"

typedef struct atxupdate_config_s
{
    const char* log;
    const char* env;
    const char* port;
    bool daemon;
} atxupdate_config_s;

typedef struct atxupdate_s atxupdate_s;

atxupdate_s* atxupdate_create(atxupdate_config_s* config);
void atxupdate_destroy(atxupdate_s**);
void atxupdate_poll(atxupdate_s*, uint32_t);

#endif /* ATXUPDATE_H */
