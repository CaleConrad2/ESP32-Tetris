idf_component_register(SRCS "server.c" "button.c" "wifi.c" "gpio.c" "main.c"
                    REQUIRES esp_wifi nvs_flash esp_event esp_netif esp_http_server esp_driver_gpio tetris freertos spiffs vfs
                    INCLUDE_DIRS "../include")