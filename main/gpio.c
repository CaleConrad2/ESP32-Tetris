#include "gpio.h"

/*
#define StartPin GPIO_NUM_15
#define RotatePin GPIO_NUM_2
#define LeftPin GPIO_NUM_4
#define DownPin GPIO_NUM_19
#define RightPin GPIO_NUM_21
*/
const static char *TAG = "GPIO_MODULE";

esp_err_t init_gpio(void) {
  const gpio_config_t StartPin_config = {.pin_bit_mask = (1ULL << StartPin),
                                         .mode = GPIO_MODE_INPUT,
                                         .pull_up_en = GPIO_PULLUP_ENABLE,
                                         .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                         .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&StartPin_config);

  const gpio_config_t RotatePin_config = {.pin_bit_mask = (1ULL << RotatePin),
                                          .mode = GPIO_MODE_INPUT,
                                          .pull_up_en = GPIO_PULLUP_ENABLE,
                                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                          .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&RotatePin_config);

  const gpio_config_t LeftPin_config = {.pin_bit_mask = (1ULL << LeftPin),
                                        .mode = GPIO_MODE_INPUT,
                                        .pull_up_en = GPIO_PULLUP_ENABLE,
                                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                        .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&LeftPin_config);

  const gpio_config_t DownPin_config = {.pin_bit_mask = (1ULL << DownPin),
                                        .mode = GPIO_MODE_INPUT,
                                        .pull_up_en = GPIO_PULLUP_ENABLE,
                                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                        .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&DownPin_config);

  const gpio_config_t RightPin_config = {.pin_bit_mask = (1ULL << RightPin),
                                         .mode = GPIO_MODE_INPUT,
                                         .pull_up_en = GPIO_PULLUP_ENABLE,
                                         .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                         .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&RightPin_config);

  gpio_install_isr_service(0);
  // registers button_isr_handler for RightPin
  gpio_isr_handler_add(StartPin, button_isr_handler, NULL);
  gpio_isr_handler_add(RotatePin, button_isr_handler, NULL);
  gpio_isr_handler_add(LeftPin, button_isr_handler, NULL);
  gpio_isr_handler_add(DownPin, button_isr_handler, NULL);
  gpio_isr_handler_add(RightPin, button_isr_handler, NULL);
  ESP_LOGI(TAG, "Button ISR with debounce installed");

  return ESP_OK;
}