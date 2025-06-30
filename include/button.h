#ifndef BUTTON_H
#define BUTTON_H

#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "pins.h"
#include "server.h"

#define DEBOUNCE_TIME_MS 50

typedef enum {
  BUTTON_START,
  BUTTON_ROTATE,
  BUTTON_LEFT,
  BUTTON_DOWN,
  BUTTON_RIGHT
} ButtonID;

extern QueueHandle_t button_event_queue;

void ButtonTask(void *pvParameters);
void button_isr_handler(void *arg);
esp_err_t init_button(void);

#endif