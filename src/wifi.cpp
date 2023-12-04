// WiFi connection and POST command

#include <stdio.h>
#include "freertos/FreeRTOS.h" // useless header here but keep until intellisense cache issue is fixed
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <wifi.h>
#include <led.h>


#define SSID "NULL"
#define PASS "NULL"

#define LOGTYPE "WIFI"

// Current wifi connection status
bool wifiConnected = false;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(LOGTYPE, "WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED: {
        esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        esp_netif_set_hostname(sta_netif, "FurryNet_ESP32");
        ESP_LOGI(LOGTYPE, "WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
    }
    case WIFI_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        setLedStatus(wifiLED, false);
        wifiConnected = false;
        ESP_LOGI(LOGTYPE, "WiFi lost connection | reconnecting ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        setLedStatus(wifiLED, true);
        wifiConnected = true;
        char ip_str[16];
        ESP_LOGI(LOGTYPE, "Device was assigned IP: %s\n", esp_ip4addr_ntoa(&((ip_event_got_ip_t *)event_data)->ip_info.ip, ip_str, sizeof(ip_str)));
        break;
    default:
        break;
    }
}

/* Setup WLAN or WIFI Connection */
void setup_wlan()
{
    nvs_flash_init();
    esp_netif_init();                    
    esp_event_loop_create_default();     
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); 
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS,
            .threshold {
                .authmode = WIFI_AUTH_WPA2_ENTERPRISE
            }}};
    //ESP_IF_WIFI_STA
    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
    uint8_t MacAddr[] = {0xFE, 0xED, 0xDE, 0xAD, 0xBE, 0xEF};
    esp_wifi_set_mac(WIFI_IF_STA, MacAddr);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}