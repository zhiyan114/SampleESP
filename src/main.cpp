#include <esp_log.h>

void setupSentry();
void setup_wlan();
void main_loader();

extern "C" {
    void app_main() {
        // Sentry should be the first to load, but it doesn't work w/o wifi so...
        setup_wlan();
        main_loader();
    }
}


// This function setups the wifi connection
void setup_wlan() {
    
}

// This function should load the main code
void main_loader() {
    ESP_LOGI("main", "Hello world!");
}