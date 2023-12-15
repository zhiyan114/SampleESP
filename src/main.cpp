#include <esp_log.h>
#include <wifi.h>
#include <led.h>
#include <Usensor.h>
#include <display.h>
#include <client.h>


void main_loader();
void data_render(void *pvParameters);
void testLED();

extern "C" {
    void app_main() {
        // Setup all the components
        led_init();
        display_init();
        ultrasonic_init();
        setup_wlan();
        // Run component startup scripts
        testLED();
        // Run the main functions
        main_loader();
        xTaskCreate(client_setup, "client_loader", 4096, NULL, 5, NULL);
        xTaskCreate(data_render, "data_render", 4096, NULL, 5, NULL);
    }
}

// Code turns on the LED for 3 seconds, ensuring it init successfully
void testLED() {
    ESP_LOGI("main", "LED is on...");
    setLedStatus(wifiLED | sensorLED, true);
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    ESP_LOGI("main", "LED is off...");
    setLedStatus(wifiLED | sensorLED, false);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// This function should load the main code
void main_loader() {
    display_write_page("Status", 0, true);
    ESP_LOGI("main", "Hello world!");
}

// This function handles data rendering for external modules
void data_render(void *pvParameters) {
    // Page 0 is the for header
    // Page 1 is for the wifi status
    // Page 2 is for wifi IP
    // Page 3 is for the server connection status
    // Page 4 is for the ultrasonic sensor status

    while(1) {
        // Page 4 render stuff
        double distance = Usensor_distance();
        char distanceStr[17];
        sprintf(distanceStr, "U-Sonic: %.0fcm", distance);
        display_write_page(distanceStr, 4, false);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}