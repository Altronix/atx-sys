#include "updater.h"

void
updater_init(updater_s* updater, const char* data, uint32_t len)
{
    memset(updater, 0, sizeof(updater_s));
    updater->ipc = 1;
}

int
updater_write(updater_s* updater, void* data, uint32_t len)
{
    updater->len += len;
    return len;
}

void
updater_free(updater_s* updater)
{
    memset(updater, 0, sizeof(updater_s));
}

bool
updater_active(updater_s* updater)
{
    return updater->ipc ? true : false;
}

enum UPDATER_STATUS
updater_status(updater_s* updater)
{
    return updater->status;
}
