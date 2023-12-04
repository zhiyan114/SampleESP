#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <client.h>
#include <esp_log.h>
#include <led.h>
#include <sensor.h>
#include <sysinfo.h>

#define SERVER_IP "NULL"
#define SERVER_PORT 42069

#define LOGTYPE "CLIENT"


// Main socket page file for the client
int main_sock;

char* concatStrings(const char* str1, const char* str2, size_t* len);
// Main function to handle TCP client connection
void client_setup(void * params) {
    while (1)
    {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(SERVER_PORT);

        while (1)
        {
            // Socket Objects
            main_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
            if (main_sock < 0)
            {
                ESP_LOGE(LOGTYPE, "Unable to create socket: errno %d", errno);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                continue;
            }

            // Socket connection stuff
            if (connect(main_sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in)) == 0)
            {
                ESP_LOGI(LOGTYPE, "Successfully connected to server");

                // TODO: Add your code for handling communication with the server here
            }
            else
            {
                ESP_LOGE(LOGTYPE, "Socket unable to connect: errno %d", errno);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                continue;
            }

            // Handle disconnection
            int bsize;
            do
            {
                setLedStatus(sensorLED, true);
                char buf[100];
                bsize = recv(main_sock, buf, sizeof(buf), 0); // This shouldn't do anything lol. It's just there to pull conn status ig.
                // Connection checks
                if(bsize < 0)
                    if (true) { // errno != EAGAIN && errno != EWOULDBLOCK
                        ESP_LOGE(LOGTYPE, "Error occurred during recv: errno %d", errno);
                        break;
                    }
                // Handle server requests
                if(strcmp("request_distance",buf) == 0) {
                    ESP_LOGE(LOGTYPE, "Server Request distance...");
                    float distance = sensor_distance();
                    int len = snprintf(NULL, 0, "%f", distance);
                    char *result = (char*)malloc(sizeof(char)*(len + 1));
                    snprintf(result, len + 1, "%f", distance);
                    const char* initalMsg = "Distance (cm): ";
                    size_t finalLen = 0;
                    char* finalMsg = concatStrings(initalMsg, result, &finalLen);
                    free(result);
                    send(main_sock, finalMsg, finalLen, 0);
                    free(finalMsg);
                    continue;
                } else if (strcmp("system_info",buf) == 0) {
                    ESP_LOGE(LOGTYPE, "Server Request system info...");
                    float SysTemp = getSoCTemp();
                    u_int32_t minFree = 0;
                    u_int32_t freeHeap = 0;
                    getHeapInfo(&freeHeap, &minFree);
                    char response[250];
                    sprintf(response, "SoC Temperature: %.2f°C\r\nCurrent Free Heap: %u\r\nMin Free Heap: %u", SysTemp, minFree, freeHeap);
                    send(main_sock, response, strlen(response)+1, 0);
                    continue;
                }
                ESP_LOGE(LOGTYPE, "Unexpected Command Received: %s", buf);
                const char* failedMsg = "Invalid Command Received";
                send(main_sock, failedMsg, strlen(failedMsg)+1, 0);
                vTaskDelay(100 / portTICK_PERIOD_MS);
            } while (bsize >= 0);
            setLedStatus(sensorLED, false);
            ESP_LOGI(LOGTYPE, "Server Disconnected (5s cooldown before reconnect)...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            ESP_LOGI(LOGTYPE, "Reconnecting to the server...");
            close(main_sock);
        }
    }

}

// Function that concat two strings

char* concatStrings(const char* str1, const char* str2, size_t* len) {
    // Calculate the length of the concatenated string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    (*len) = len1 + len2 + 1;  // +1 for the null terminator

    // Allocate memory for the concatenated string
    char* result = (char*)malloc(*len);

    if (result == NULL) {
        // Memory allocation failed
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the first string into the result buffer
    strcpy(result, str1);

    // Concatenate the second string onto the result buffer
    strcat(result, str2);

    return result;
}
