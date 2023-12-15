// HC-SR04 Ultrasonic sensor codes
#include <Usensor.h>
#include <led.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_timer.h"

#define TRIG_PIN GPIO_NUM_23
#define ECHO_PIN GPIO_NUM_22

static volatile uint32_t start_time = 0;
static volatile uint32_t end_time = 0;

// = xSemaphoreCreateBinary();
static QueueHandle_t echo_sem = xSemaphoreCreateBinary();

static void IRAM_ATTR echo_isr_handler(void* arg) {
    if (gpio_get_level(ECHO_PIN) == 1) {
        start_time = esp_timer_get_time();
    } else {
        end_time = esp_timer_get_time();
        xSemaphoreGiveFromISR(echo_sem, NULL);
    }
}
// Initalize the sensor's GPIO and stuff
void ultrasonic_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << TRIG_PIN) | (1ULL << ECHO_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(ECHO_PIN, echo_isr_handler, (void*) ECHO_PIN);
}

// The main code to get the distance of the sensor
double Usensor_distance() {
    gpio_set_level(TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(2));

    gpio_set_level(TRIG_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(TRIG_PIN, 0);

    if (xSemaphoreTake(echo_sem, pdMS_TO_TICKS(500))) {
        uint32_t duration = end_time - start_time;
        // Speed of sound = 343 meters/second (0.0343 cm/microsecond)
        float distance = ((float)duration / 2.0) * 0.0343; // Convert to Centimeter
        return distance;
    } else {
        return -1.0; // Timeout
    }
}