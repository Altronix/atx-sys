#include "log.h"
#include "updater.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int
set_blocking(updater_ipc_handle fd, bool block)
{
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -EFAULT;
    return fcntl(fd, F_SETFL, block ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
}

void
updater_init(updater_s* updater, const char* data, uint32_t len)
{
    int err;
    log_info("(UPDATE) Starting update [%.*s]", len, data);
    if (updater->ipc) ipc_end(updater->ipc);
    memset(updater, 0, sizeof(updater_s));
    updater->ipc = ipc_inst_start_ext(SOURCE_WEBSERVER, len, data, false);
    if (updater->ipc < 0) {
        log_fatal("(UPDATE) Failure to initiate swupdate IPC command!");
        exit(-1);
    }
    err = set_blocking(updater->ipc, false);
    if (err) log_warn("(UPDATE) Failure to set non blocking mode!");
}

int
updater_write(updater_s* updater, void* data, uint32_t len)
{
    int written = write(updater->ipc, data, len);
    if (written != len) {
        if (!(errno == EAGAIN || errno == EWOULDBLOCK)) {
            log_error("(UPDATE) write io error [%s]", strerror(errno));
        } else {
            log_debug("(UPDATE) write io error [%s]", strerror(errno));
        }
        usleep(100);
        if (written < 0) written = 0;
    }
    updater->len += written;
    return written;
}

void
updater_free(updater_s* updater)
{
    log_info("(UPDATE) free");
    if (updater->ipc) ipc_end(updater->ipc);
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
