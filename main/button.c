#include "button.h"

static const char *TAG = "BUTTON_MODULE";
QueueHandle_t button_event_queue;
TimerHandle_t debounce_timer;
TimerHandle_t down_repeat_timer;

void ButtonTask(void *pvParameters) {
  ButtonID received;
  // while (1) keeps the task alive forever, different from app_main while (1)
  // task starts executing, hits xQueueReceive, blocks, then wakes up when
  // something is sent to the queue, handles it, then loops back to
  // xQueueReceive
  while (1) {
    // task does nothing until something is put in this queue
    if (xQueueReceive(button_event_queue, &received, portMAX_DELAY)) {
      char c = '?';
      switch (received) {
      case BUTTON_START:
        ESP_LOGI(TAG, "Start");
        c = 'S';
        break;
      case BUTTON_ROTATE:
        ESP_LOGI(TAG, "Rotate");
        c = 'U';
        break;
      case BUTTON_LEFT:
        ESP_LOGI(TAG, "Left");
        c = 'L';
        break;
      case BUTTON_DOWN:
        ESP_LOGI(TAG, "Down");
        c = 'D';
        break;
      case BUTTON_RIGHT:
        ESP_LOGI(TAG, "Right");
        c = 'R';
        break;
      }
      // send char to browser
      ws_send_char(c);
    }
  }
}

// ISR: handle interrupt when button is pressed
// restarts debounce_timer
// avoids bounce noise
void IRAM_ATTR button_isr_handler(void *arg) {
  xTimerResetFromISR(debounce_timer, NULL);
}

// Debounce timer callback
// this gets called 50 ms after button_isr_handler
// use info from here from button presses
// this is the button press debounced info
void debounce_timer_callback(TimerHandle_t myTimer) {
  if (gpio_get_level(StartPin) == 0) {
    ButtonID id = BUTTON_START;
    xQueueSendFromISR(button_event_queue, &id, NULL);
  }
  if (gpio_get_level(RotatePin) == 0) {
    ButtonID id = BUTTON_ROTATE;
    xQueueSendFromISR(button_event_queue, &id, NULL);
  }
  if (gpio_get_level(LeftPin) == 0) {

    ButtonID id = BUTTON_LEFT;
    xQueueSendFromISR(button_event_queue, &id, NULL);
  }
  if (gpio_get_level(DownPin) == 0) {
    // allows for holding down the button
    ButtonID id = BUTTON_DOWN;
    xQueueSendFromISR(button_event_queue, &id, NULL);
    xTimerStartFromISR(down_repeat_timer, NULL);
  }
  if (gpio_get_level(RightPin) == 0) {

    ButtonID id = BUTTON_RIGHT;
    xQueueSendFromISR(button_event_queue, &id, NULL);
  }
}
// gets repeatedly called until xTimerStop runs
void down_repeat_callback(TimerHandle_t myTimer) {
  if (gpio_get_level(DownPin) == 0) {
    ButtonID id = BUTTON_DOWN;
    xQueueSendFromISR(button_event_queue, &id, NULL);
  } else {
    xTimerStop(myTimer, 0);
  }
}

esp_err_t init_button(void) {
  button_event_queue = xQueueCreate(10, sizeof(ButtonID));
  xTaskCreate(ButtonTask, "ButtonTask", 2048, NULL, 10, NULL);
  // create debounce timer
  // debounce_timer_callback is called when timer expires (every 50ms)
  debounce_timer = xTimerCreate("debounce_timer",
                                pdMS_TO_TICKS(DEBOUNCE_TIME_MS), // 50ms
                                pdFALSE, NULL, debounce_timer_callback);
  down_repeat_timer = xTimerCreate("down_repeat", pdMS_TO_TICKS(50), pdTRUE,
                                   NULL, down_repeat_callback);
  return ESP_OK;
}