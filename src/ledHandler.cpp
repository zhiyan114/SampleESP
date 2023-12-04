#include <led.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define WIFI_LED GPIO_NUM_2 // On-board LED
#define SENSOR_LED GPIO_NUM_25 // External LED

// Initalize the GPIO and led configs
void led_init() {
    gpio_config_t io_conf_onboard = {
        .pin_bit_mask = (1ULL<<WIFI_LED),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf_onboard);

    // Configure external LED as output
    gpio_config_t io_conf_external = {
        .pin_bit_mask = (1ULL<<SENSOR_LED),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf_external);
}

// Takes in the led that the operation will be performed on and the mode (turning on or off the LED)
void setLedStatus(int LedType, bool enabled) {
    if(LedType & wifiLED)
        gpio_set_level(WIFI_LED, enabled ? 1 : 0);
    if(LedType & sensorLED)
        gpio_set_level(SENSOR_LED, enabled ? 1 : 0);
}
