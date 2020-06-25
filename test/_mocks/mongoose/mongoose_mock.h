// Copyright (c) 2019-2020 Altronix Corp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MOCK_MONGOOSE_H
#define MOCK_MONGOOSE_H

#include "parse_http.h"

#include "mongoose.h"
#undef LIST_INIT
#undef closesocket
#undef INVALID_SOCKET

#ifdef __cplusplus
extern "C"
{
#endif

    // State of mock event
    typedef struct mongoose_mock_event
    {
        struct mg_connection* c;
        int ev;
        void* p;
        struct http_message message;
        uint32_t curr_header_idx;
        char request[2048];
    } mongoose_mock_event;

    typedef struct mongoose_mock_outgoing_data
    {
        uint32_t l;
        char mem[2048];
    } mongoose_mock_outgoing_data;

    // Any outgoing responses
    typedef mongoose_parser_context mongoose_mock_response;

    void mongoose_spy_init();
    void mongoose_spy_deinit();
    void mongoose_spy_incoming_events_flush();
    void mongoose_spy_outgoing_data_flush();

    void mongoose_spy_event_request_push(
        const char* auth,
        const char* meth,
        const char* path,
        const char* data);

    void mongoose_mock_outgoing_data_destroy(
        mongoose_mock_outgoing_data** data_p);
    void mongoose_spy_event_close_push(int handle);
    mongoose_mock_outgoing_data* mongoose_spy_outgoing_data_pop(int n);
    mongoose_parser_context* mongoose_spy_response_pop();
    void mongoose_spy_set_ev_handler(mg_event_handler_t* ev, void*);
    void mongoose_mock_response_destroy(mongoose_mock_response** resp_p);

#ifdef __cplusplus
}
#endif
#endif
