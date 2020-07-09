#include "http_route_update.h"
#include "log.h"

void
update_handler(struct mg_connection* c, int ev, void* p, void* user_data)
{
    switch (ev) {
        case MG_EV_HTTP_MULTIPART_REQUEST:
            log_trace("%06s %04s", "(HTTP)", "multipart request");
            break;
        case MG_EV_HTTP_PART_BEGIN: {
            log_trace("%06s %04s", "(HTTP)", "part begin");
            struct mg_http_multipart_part* mp = p;
            http_s* http = user_data;
            updater_init(&http->updater, mp->file_name, strlen(mp->file_name));
            mp->user_data = http;
        } break;
        case MG_EV_HTTP_PART_DATA: {
            log_trace("%06s %04s", "(HTTP)", "part data");
            int l;
            struct mg_http_multipart_part* mp = p;
            http_s* http = user_data;
            if (!(http && updater_active(&http->updater))) break;
            if (!(updater_status(&http->updater) == UPDATER_STATUS_OK)) break;
            l = updater_write(&http->updater, (void*)mp->data.p, mp->data.len);
            mp->num_data_consumed = l;
        } break;
        case MG_EV_HTTP_PART_END: {
            log_trace("%06s %04s", "(HTTP)", "part end");
            struct mg_http_multipart_part* mp = p;
            http_s* http = user_data;
            int code = updater_status(&http->updater) == UPDATER_STATUS_FAIL
                           ? 500
                           : 200;
            c->flags |= MG_F_SEND_AND_CLOSE;
            http_printf_json(
                c,
                code,
                "{\"error\":\"%s\",\"received\":%d,\"from\":\"%s\"}",
                http_error_message(code),
                http->updater.len,
                mp->file_name);
            mp->user_data = NULL;
            updater_free(&http->updater);
        } break;
        case MG_EV_HTTP_MULTIPART_REQUEST_END:
            log_trace("%06s %04s", "(HTTP)", "multipart request end");
            break;
    }
}
