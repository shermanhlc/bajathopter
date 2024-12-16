#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"


#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "mqtt.h"
#include "cJSON.h"
#include "transmitter.h"
#include "esp_http_client.h"

#define ARRAY_SIZE 100

Message_t messageArray[ARRAY_SIZE];
int currentIndex = 0;

static const char *TAG = "Transmitter";


extern QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphoreFull;

TaskHandle_t queueReceiveTaskHandle = NULL;

esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP Response: %.*s", evt->data_len, (char *)evt->data);
        break;
    default:
        break;
    }
    return ESP_OK;
}


static void transmitter_queue_receive_task(void) {
    Message_t receivedMessage;

    for (;;) {
        if (xQueueReceive(xQueue, &receivedMessage, portMAX_DELAY) == pdPASS) {
                messageArray[currentIndex++] = receivedMessage;
                ESP_LOGI(TAG, "Handling buffer add");
                ESP_LOGI(TAG, "BUFFER SIZE: %d", currentIndex);
                if (currentIndex == ARRAY_SIZE) {
                    xSemaphoreGive(xSemaphoreFull);
                }
        }
    }
}

static void array_full_task(void) {
    char* jsonString = NULL;

    while (1) {
        // Wait until signal indicating array is full
        if (xSemaphoreTake(xSemaphoreFull, portMAX_DELAY) == pdTRUE) {

            ESP_LOGI(TAG, "Preparing JSON Data");
            cJSON* root = cJSON_CreateObject();
            cJSON *messagesArray = cJSON_CreateArray();

            for (int i = 0; i < ARRAY_SIZE; i++) {
                cJSON *messageObj = cJSON_CreateObject();
                cJSON_AddNumberToObject(messageObj, "sensor_id", messageArray[i].id);
                cJSON_AddNumberToObject(messageObj, "sensor_voltage", messageArray[i].value);
                cJSON_AddNumberToObject(messageObj, "timestamp", messageArray[i].timestamp);
                cJSON_AddItemToArray(messagesArray, messageObj);
            }

            cJSON_AddItemToObject(root, "voltages", messagesArray);

            jsonString = cJSON_PrintUnformatted(root);

            currentIndex = 0;
            xSemaphoreTake(xSemaphoreFull, 0); // Reset semaphore

            cJSON_Delete(root);

            /* HTTP POST code */
            esp_http_client_config_t config = {
                .url = "http://192.168.4.254/sensor",
                .event_handler = http_event_handler,
            };

            esp_http_client_handle_t client = esp_http_client_init(&config);

            // SET POST details
            esp_http_client_set_url(client, "http://192.168.4.254/sensor");
            esp_http_client_set_method(client, HTTP_METHOD_POST);
            esp_http_client_set_header(client, "Content-Type", "application/json");
            esp_http_client_set_post_field(client, jsonString, strlen(jsonString));

            esp_err_t err = esp_http_client_perform(client);

            if (err == ESP_OK) {
                ESP_LOGI(TAG, "HTTP POST success");
            } else {
                ESP_LOGE(TAG, "HTTP POST Fail: %s", esp_err_to_name(err));
            }
            
            esp_http_client_cleanup(client);
        }
    }
}

void transmitter_init(void) {
    xSemaphoreFull = xSemaphoreCreateBinary();
    xTaskCreate(transmitter_queue_receive_task, "transmitter_queue_receive_task", 16384, NULL, tskIDLE_PRIORITY + 1, &queueReceiveTaskHandle);
    xTaskCreate(array_full_task, "array_full_task", 16384, NULL, tskIDLE_PRIORITY + 2, NULL);

}