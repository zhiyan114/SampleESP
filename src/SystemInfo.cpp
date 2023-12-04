#include <sysinfo.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
// #include <driver/temperature_sensor.h>
#include "esp_system.h"
#include "esp_spi_flash.h"

#define LOGTYPE "SysInfo"

// temperature_sensor_handle_t temp_handle = NULL;
// void initTempSensor() {
//     temperature_sensor_config_t temp_sensor = {
//         .range_min = 0,
//         .range_max = 100,
//     };
//     temperature_sensor_install(&temp_sensor, &temp_handle);
// }

// // Get the SoC Temperature (in celsius)
// float getSoCTemp() {
//     temperature_sensor_enable(temp_handle);
//     float tsens_out;
//     temperature_sensor_get_celsius(temp_handle, &tsens_out);
//     printf("Temperature in %f °C\n", tsens_out);
//     ESP_LOGI(LOGTYPE, "Temperature Read: %.2f degrees Celsius", tsens_out);
//     temperature_sensor_disable(temp_handle);
//     return tsens_out;
// }

// Get the basic memory usage info
void getHeapInfo(uint32_t * freeHeap, uint32_t * minFree) {
    (*freeHeap) = esp_get_free_heap_size();
    (*minFree) = esp_get_minimum_free_heap_size();
    ESP_LOGI(LOGTYPE, "Free Heap Read: %lu", *freeHeap);
    ESP_LOGI(LOGTYPE, "Minimum Free Heap Read: %lu", *freeHeap);
}