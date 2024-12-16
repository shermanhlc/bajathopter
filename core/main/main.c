/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "time.h"

#include "wifi.h"
#include "mqtt_handler.h"
#include "led.h"


static const char *TAG = "Core";

typedef struct {
    uint8_t id;
    float value;
    int64_t timestamp;
} Message_t;

QueueHandle_t xQueue;

void read_voltage_task(void *taskParameters) {
    Message_t messageToSend;

    for (;;) {
        // Since we don't have actual sensors, just generate a random sensor ID and voltage
        // In reality, there would be one task per sensor doing this, or an GPIO interrupt
        uint8_t id = (uint8_t)(rand() % 20) + 1;
        float value = (float)rand() / RAND_MAX * 5;
        // Get current timestamp - note, doesn't use NTP, so this is not actually accurate
        // But can be used to gauge time intervals
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        messageToSend.id = id;
        messageToSend.value = value;
        messageToSend.timestamp = time_us;
        
        if (xQueueSend(xQueue, &messageToSend, portMAX_DELAY) == pdPASS) {
            ESP_LOGI(TAG, "Read from sensor %d", id);
        } else {
            ESP_LOGI(TAG, "Failed to send data from sensor %d", id);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void transmit_voltage_task(void *taskParameters) {
    Message_t receivedMessage;

    for (;;) {
        // Receive message and send to MQTT
        if (xQueueReceive(xQueue, &receivedMessage, portMAX_DELAY) == pdPASS) {
            // Received message
            mqtt_handler_publish(receivedMessage.id, receivedMessage.value, receivedMessage.timestamp);
        }
    }
}


static void blink_task(void *taskParameters) {
    for (;;) {
        blink_led();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        clear_led();
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    srand(time(NULL));
    init_led();
    wifi_init_sta();


    // Init MQTT
    mqtt_handler_initialize("mqtt://192.168.4.254:1883");
    mqtt_handler_start();

    xQueue = xQueueCreate(30, sizeof(Message_t));
    if (xQueue == NULL) {
        ESP_LOGE(TAG, "Failed to create queue. Terminating");
        return;
    }

    xTaskCreate(blink_task, "blink_task", 1024, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(read_voltage_task, "read_voltage_task", 4096, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(transmit_voltage_task, "transmit_voltage_task", 4096, NULL, tskIDLE_PRIORITY, NULL);

}
