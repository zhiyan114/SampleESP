#include <esp_log.h>
#include <wifi.h>
#include <client.h>


void main_loader();

extern "C" {
    void app_main() {
        setup_wlan();
        main_loader();
        xTaskCreate(tcp_client_setup, "client_loader", 4096, NULL, 5, NULL);
    }
}



// This function should load the main code
void main_loader() {
    ESP_LOGI("main", "Hello world!");
}