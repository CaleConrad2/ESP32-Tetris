#include "server.h"
#include "esp_check.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "esp_vfs.h"
#include <string.h>

static const char *TAG = "SERVER_MODULE";

static httpd_handle_t s_server = NULL;
static int s_ws_client_fd = -1;

// mime
static const char *get_mime_type(const char *path) {
  if (strstr(path, ".html"))
    return "text/html";
  if (strstr(path, ".css"))
    return "text/css";
  if (strstr(path, ".js"))
    return "application/javascript";
  if (strstr(path, ".svg"))
    return "image/svg+xml";
  if (strstr(path, ".png"))
    return "image/png";
  if (strstr(path, ".ico"))
    return "image/x-icon";
  if (strstr(path, ".gif"))
    return "image/gif";
  return "text/plain";
}

static esp_err_t static_file_handler(httpd_req_t *req) {
  char filepath[ESP_VFS_PATH_MAX + 32] = "/spiffs";
  if (strcmp(req->uri, "/") == 0)
    strlcat(filepath, "/tetris.html", sizeof(filepath));
  else
    strlcat(filepath, req->uri, sizeof(filepath));

  ESP_LOGI(TAG, "→ %s", filepath);

  FILE *f = fopen(filepath, "r");
  if (!f) {
    ESP_LOGW(TAG, "File not found");
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, get_mime_type(filepath));

  char buf[1024];
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
    if (httpd_resp_send_chunk(req, buf, n) != ESP_OK) {
      fclose(f);
      return ESP_FAIL;
    }

  fclose(f);
  httpd_resp_send_chunk(req, NULL, 0);
  return ESP_OK;
}

// websocket handler
static esp_err_t ws_handler(httpd_req_t *req) {
  if (req->method == HTTP_GET) {
    s_ws_client_fd = httpd_req_to_sockfd(req);
    ESP_LOGI(TAG, "WebSocket connected (fd=%d)", s_ws_client_fd);
    return ESP_OK;
  }

  // websocket frame
  httpd_ws_frame_t frame = {.type = HTTPD_WS_TYPE_TEXT};
  esp_err_t ret = httpd_ws_recv_frame(req, &frame, 0); 
  if (ret != ESP_OK)
    return ret;
  frame.payload = malloc(frame.len + 1);
  ret = httpd_ws_recv_frame(req, &frame, frame.len);
  free(frame.payload);
  return ret;
}

esp_err_t ws_send_char(char c) {
  if (s_ws_client_fd < 0)
    return ESP_ERR_INVALID_STATE;

  httpd_ws_frame_t frame = {
      .type = HTTPD_WS_TYPE_TEXT,
      .payload = (uint8_t *)&c,
      .len = 1,
  };
  return httpd_ws_send_frame_async(s_server, s_ws_client_fd, &frame);
}

httpd_handle_t start_server(void) {
  httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
  cfg.uri_match_fn = httpd_uri_match_wildcard;

  if (httpd_start(&s_server, &cfg) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start server");
    return NULL;
  }

  static const httpd_uri_t static_uri = {
      .uri = "/*",
      .method = HTTP_GET,
      .handler = static_file_handler,
  };

  static const httpd_uri_t ws_uri = {
      .uri = "/buttons",
      .method = HTTP_GET,
      .handler = ws_handler,
      .is_websocket = true,
  };
  httpd_register_uri_handler(s_server, &ws_uri);
  httpd_register_uri_handler(s_server, &static_uri);

  ESP_LOGI(TAG, "HTTP & WS server started");
  return s_server;
}

// spiffs mount
esp_err_t mount_spiffs(void) {
  esp_vfs_spiffs_conf_t cfg = {
      .base_path = "/spiffs",
      .partition_label = "spiffs",
      .max_files = 8,
      .format_if_mount_failed = false,
  };
  ESP_RETURN_ON_ERROR(esp_vfs_spiffs_register(&cfg), TAG, "SPIFFS mount fail");

  size_t total = 0, used = 0;
  esp_spiffs_info(cfg.partition_label, &total, &used);
  ESP_LOGI(TAG, "SPIFFS: %u / %u bytes used", used, total);
  return ESP_OK;
}
