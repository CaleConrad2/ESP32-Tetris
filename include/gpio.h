#ifndef GPIO_H
#define GPIO_H

#include "button.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "pins.h"


esp_err_t init_gpio(void);

#endif