#ifndef WIFI_H
#define WIFI_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "lwip/err.h"
#include "lwip/sys.h"

/* WiFi configuration through project configuration*/
#define ESP_WIFI_SSID                       CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS                       CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY                   CONFIG_ESP_MAXIMUM_RETRY
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD   WIFI_AUTH_WPA2_PSK



static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void wifi_init_sta(void);

#endif /* WIFI_H */