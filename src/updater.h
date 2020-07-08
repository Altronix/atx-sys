#ifndef UPDATER_H_
#define UPDATER_H_

#include "private.h"
#if EMU == FALSE
#include <network_ipc.h>
#include <progress.h>
typedef int updater_ipc_handle;
#else
typedef int updater_ipc_handle;
#endif

typedef struct updater_s
{
    updater_ipc_handle ipc;
    uint32_t len;
} updater_s;

void updater_init(updater_s* updater, const char* data, uint32_t len);
int updater_write(updater_s* updater, void* data, uint32_t len);
void updater_free(updater_s* updater);
bool updater_active(updater_s* updater);

#endif /* UPDATER_H_ */
