#include "wifi.h"

#include "esp_event.h"
#include "esp_wifi.h"

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

static const char *TAG = "WIFI_MODULE";

/* To implement:
   - User inputting SSID, password rather than hard coded
   - Pulling config from NVS rather than inputting SSID, password
   - Refactor error handlers
*/

// this is what happens when the handler gets called
static esp_event_handler_instance_t ip_event_handler;
static void ip_handler(void *event_handler_arg, esp_event_base_t event_base,
                       int32_t event_id, void *event_data) {
  ESP_LOGI(TAG, "Handling IP event");
  if (event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event_ip = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "Got IP:" IPSTR "\n", IP2STR(&event_ip->ip_info.ip));
  }
}

esp_err_t wifi_init() {
  wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
  // wifi_config_t is a union of structs
  static wifi_config_t wifi_conf = {.sta = {.ssid = WIFI_SSID,
                                            .password = WIFI_PASSWORD,
                                            .scan_method = WIFI_FAST_SCAN,
                                            .bssid_set = 0}};

  // must capture the return value
  // This API must be called before all other WiFi API can be called

  esp_err_t err;

  err = esp_wifi_init(&init_config);
  if (err != ESP_OK)
    return err;

  err = esp_wifi_set_mode(WIFI_MODE_STA);
  if (err != ESP_OK)
    return err;

  err = esp_wifi_set_config(WIFI_IF_STA, &wifi_conf);
  if (err != ESP_OK)
    return err;

  esp_netif_create_default_wifi_sta();

  err = esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, ip_handler, NULL, &ip_event_handler);
  if (err != ESP_OK)
    return err;

  err = esp_wifi_start();
  if (err != ESP_OK)
    return err;

  err = esp_wifi_connect();
  if (err != ESP_OK)
    return err;

  return ESP_OK;
};
