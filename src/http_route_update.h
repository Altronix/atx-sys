#ifndef HTTP_ROUTE_UPLOAD_H
#define HTTP_ROUTE_UPLOAD_H

#include "http.h"
#include <progress_ipc.h>

void update_handler(struct mg_connection* c, int ev, void* p, void* user_data);

#endif /* HTTP_ROUTE_UPLOAD_H */
