#include <sysinfo.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"

#define LOGTYPE "SysInfo"

// Get the basic cpu usage info
void getCpuInfo(char*RunTime, char*TaskList) {
	vTaskGetRunTimeStats(RunTime);
	ESP_LOGI(LOGTYPE, "CPU Runtime: %s", RunTime);
	vTaskList(TaskList);
	ESP_LOGI(LOGTYPE, "CPU Tasklist: %s", TaskList);
}

// Get the basic memory usage info
void getHeapInfo(uint32_t * freeHeap, uint32_t * minFree) {
    (*freeHeap) = esp_get_free_heap_size();
    (*minFree) = esp_get_minimum_free_heap_size();
    ESP_LOGI(LOGTYPE, "Free Heap Read: %lu", *freeHeap);
    ESP_LOGI(LOGTYPE, "Minimum Free Heap Read: %lu", *freeHeap);
}