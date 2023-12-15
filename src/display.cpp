#include <display.h>
#include <string.h>
#include "driver/i2c.h"
#include <ssd1306.h>
#include <font8x8_basic.h>
#include <esp_log.h>
#include <math.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <queue.h>

#define SDA_PIN 27
#define SCL_PIN 26

#define TAG "display"

SSD1306_t dev;

// Initalize the display pins
void display_clear();
void display_write_queue(void *pvParameters);
void display_init()
{
	i2c_master_init(&dev, SDA_PIN, SCL_PIN, 0);
	ssd1306_init(&dev, 128, 64);
	display_clear();
	xTaskCreate(display_write_queue, "display_write_queue", 4096, NULL, 5, NULL);
}

// Clean the diasplay
void display_clear() {
	ssd1306_clear_screen(&dev, false);
}

// Render text on the display
void display_text(const char* text) {
	uint8_t text_len = strlen(text);
    ssd1306_display_text(&dev, 0, const_cast<char*>(text), text_len, false);
}

// Write text to a specific line on the display (isCenter is used to center the text on the line)
QueueHandle_t writePageQueue = xQueueCreate(32, sizeof(displayQueue_t));
void display_write_page(const char* text, int page, bool isCenter) {
	// Get the length and allocate the space
	size_t text_len = strlen(text); // Each line only supports 16 characters
	char* strArr = (char*)malloc(sizeof(char) * (text_len + 1));
	text_len = text_len > 16 ? 16 : text_len;

	// Check if the text needs to be centered
	if(isCenter) {
		int padLen = ceil((16 - text_len) / 2);
		for(int i = 0; i < padLen; i++)
			strArr[i] = ' ';
		for(int i = padLen; i < padLen+text_len; i++)
			strArr[i] = text[i - padLen];
		text_len+=padLen;
		strArr[text_len] = '\0';
	} else strncpy(strArr, text, text_len + 1);

	// Add it to the queue
	displayQueue_t data;
	data.text = strArr;
	data.page = page;
	data.text_len = text_len;
	xQueueSend(writePageQueue, &data, 0);
}

/*
Internal function to handle writeLineQueue
Executing ssd1306 display command simultaneously causes the display to glitch out
*/
void display_write_queue(void *pvParameters) {
	while(1) {
		displayQueue_t data;
		if(xQueueReceive(writePageQueue, &data, portMAX_DELAY)) {
			/* Handler Stuff Here */
			ssd1306_clear_line(&dev, data.page, false);
			vTaskDelay(30 / portTICK_PERIOD_MS);
    		ssd1306_display_text(&dev, data.page, data.text, data.text_len, false);
			/* End Handler Stuff */
			free(data.text);
		}
		else
			vTaskDelay(30 / portTICK_PERIOD_MS);
	}
}