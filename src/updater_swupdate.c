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

void
updater_progress_init(updater_progress_s* u, updater_progress_callbacks_s* cb)
{
    memset(u, 0, sizeof(updater_progress_s));
    u->cb = cb;
}

void
updater_progress_free(updater_progress_s* u)
{
    if (u->ipc > 0) close(u->ipc);
    memset(u, 0, sizeof(updater_progress_s));
}

void
updater_progress_poll(updater_progress_s* u)
{
    int err;
    struct progress_msg msg;
    if (u->ipc > 0) {
        // receive an ipc message
        err = progress_ipc_receive(&u->ipc, &msg);
        if (!(err == sizeof(msg))) {
            log_warn("(UPDATE) progress failed to receive proper message");
            log_warn("(UPDATE) expect %d / received %d", sizeof(msg), err);
            return;
        }
        if (msg.status != u->status || msg.status == FAILURE) {
            u->status = msg.status;
            if (u->cb->status) u->cb->status(u->status);
        }
        if (msg.source != u->source) {
            u->source = msg.source;
            if (u->cb->source) u->cb->source(u->source);
        }
        if (msg.infolen) {
            if (u->cb->info) u->cb->info(msg.info, msg.infolen);
        }
        if ((msg.cur_step != u->step || msg.cur_percent != u->percent) &&
            msg.cur_step) {
            u->step = msg.cur_step;
            u->percent = msg.cur_percent;
            if (u->cb->step) {
                u->cb->step(
                    msg.cur_step ? msg.cur_image : "",
                    msg.cur_step,
                    msg.nsteps,
                    msg.cur_percent);
            }
        }
    } else {
        u->ipc = progress_ipc_connect(false);
        if (!(u->ipc < 0)) log_warn("(UPDATE) ipc failed to connect");
    }
}
