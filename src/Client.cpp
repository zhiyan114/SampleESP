#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <client.h>
#include <esp_log.h>

#define SERVER_IP "NULL"
#define SERVER_PORT 12345

#define LOGTYPE "CLIENT"

void tcp_client_setup(void *pvParameters) {
    struct sockaddr_in server_addr;
    int client_socket;

    while (1) {
        // Create socket
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Error creating socket\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Set up server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

        // Connect to the server
        if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            printf("Error connecting to server\n");
            close(client_socket);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Send data
        const char *message = "Hello, server!";
        send(client_socket, message, strlen(message), 0);

        // Receive data
        char buffer[1024];
        recv(client_socket, buffer, sizeof(buffer), 0);
        ESP_LOGI(LOGTYPE, "Received from server: %s\n", buffer);

        // Close the socket
        close(client_socket);

        vTaskDelay(5000 / portTICK_PERIOD_MS);  // Delay before reconnecting
    }
}