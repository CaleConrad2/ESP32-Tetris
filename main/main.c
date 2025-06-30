#include "button.h"
#include "esp_check.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "gpio.h"
#include "nvs_flash.h"
#include "server.h"
#include "wifi.h"
#include <stdio.h>

const static char *TAG = "MAIN_MODULE";

void app_main(void) {
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(mount_spiffs());
  ESP_ERROR_CHECK(wifi_init());
  httpd_handle_t server = start_server();
  if (server == NULL) {
    ESP_LOGE(TAG, "Server failed to start");
    return;
  }
  ESP_ERROR_CHECK(init_gpio());
  ESP_ERROR_CHECK(init_button());
}
