#include <sysinfo.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/temp_sensor.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define LOGTYPE "SysInfo"

// Get the SoC Temperature (in celsius)
float getSoCTemp() {
    float temp;
    esp_err_t ret = temp_sensor_read_celsius(&temp);
    if (ret == ESP_OK) {
        ESP_LOGI(LOGTYPE, "Temperature Read: %.2f degrees Celsius", temp);
        return temp;
    }
    ESP_LOGE(LOGTYPE, "Failed to read temperature");
    
    return -1;
}

// Get the basic memory usage info
void getHeapInfo(u_int32_t * freeHeap, u_int32_t * minFree) {
    (*freeHeap) = esp_get_free_heap_size();
    (*minFree) = esp_get_minimum_free_heap_size();
    ESP_LOGI(LOGTYPE, "Free Heap Read: %u", *freeHeap);
    ESP_LOGI(LOGTYPE, "Minimum Free Heap Read: %u", *freeHeap);
}