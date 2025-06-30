#ifndef HTTP_H
#define HTTP_H

#include "esp_http_server.h"
#include "esp_spiffs.h"

httpd_handle_t start_server(void);
esp_err_t mount_spiffs(void);
esp_err_t ws_send_char(char c);

#endif