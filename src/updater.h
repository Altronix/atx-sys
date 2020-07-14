#ifndef UPDATER_H_
#define UPDATER_H_

#include "private.h"
#if EMU == FALSE
#include <network_ipc.h>
#include <progress_ipc.h>
typedef int updater_ipc_handle;
typedef int updater_progress_ipc_handle;
typedef RECOVERY_STATUS UPDATER_PROGRESS_STATUS;
typedef sourcetype UPDATER_PROGRESS_SOURCE;
#else
typedef int updater_ipc_handle;
typedef int updater_progress_ipc_handle;
// clang-format off
typedef enum {} UPDATER_PROGRESS_STATUS; // TODO
typedef enum {} UPDATER_PROGRESS_SOURCE; // TODO
// clang-format on
#endif

enum UPDATER_STATUS
{
    UPDATER_STATUS_OK = 0,
    UPDATER_STATUS_FAIL = -1
};

typedef struct updater_s
{
    updater_ipc_handle ipc;
    uint32_t len;
    enum UPDATER_STATUS status;
} updater_s;

typedef struct updater_progress_callbacks_s
{
    void (*status)(void*, UPDATER_PROGRESS_STATUS);
    void (*source)(void*, UPDATER_PROGRESS_SOURCE);
    void (*info)(void*, const char*, uint32_t);
    void (*step)(
        void*,
        const char* name,
        uint8_t step,
        uint8_t total,
        uint8_t per);
} updater_progress_callbacks_s;

typedef struct updater_progress_s
{
    updater_progress_ipc_handle ipc;
    UPDATER_PROGRESS_STATUS status;
    UPDATER_PROGRESS_SOURCE source;
    updater_progress_callbacks_s* cb;
    void* ctx;
    int step;
    int percent;
} updater_progress_s;

void updater_init(updater_s* updater, const char* data, uint32_t len);
int updater_write(updater_s* updater, void* data, uint32_t len);
void updater_free(updater_s* updater);
bool updater_active(updater_s* updater);
enum UPDATER_STATUS updater_status(updater_s* updater);

void updater_progress_init(
    updater_progress_s* u,
    updater_progress_callbacks_s*,
    void* ctx);
void updater_progress_free(updater_progress_s* u);
void updater_progress_poll(updater_progress_s* u);
const char* updater_progress_status_message(UPDATER_PROGRESS_STATUS status);
const char* updater_progress_source_message(UPDATER_PROGRESS_SOURCE source);

#endif /* UPDATER_H_ */
