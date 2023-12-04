#include <esp_log.h>
#include <wifi.h>
#include <led.h>
#include <sensor.h>
#include <client.h>


void main_loader();
void testLED();

extern "C" {
    void app_main() {
        led_init();
        testLED();
        ultrasonic_init();
        setup_wlan();
        main_loader();
        xTaskCreate(client_setup, "client_loader", 4096, NULL, 5, NULL);
    }
}

// Code turns on the LED for 3 seconds, ensuring it init successfully
void testLED() {
    ESP_LOGI("main", "LED is on...");
    setLedStatus(wifiLED | sensorLED, true);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ESP_LOGI("main", "LED is off...");
    setLedStatus(wifiLED | sensorLED, false);
}

// This function should load the main code
void main_loader() {
    ESP_LOGI("main", "Hello world!");
}