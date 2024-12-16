#include "mqtt.h"
#include <stdbool.h>
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"

#define TAG "MQTT"

static esp_mqtt_client_handle_t mqtt_client;

QueueHandle_t xQueue = NULL;

void event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(client, "/topic/sensors", 1);
            ESP_LOGI(TAG, "MQTT Connected");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT Disconnected");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT published");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT Subscribed");
            break;
        case MQTT_EVENT_DATA:
            // Handle data received from subscribed topic
            ESP_LOGI(TAG, "MQTT DATA");
            const cJSON *id = NULL;
            const cJSON *value = NULL;
            const cJSON *timestamp = NULL;
            cJSON* monitor = cJSON_Parse(event->data);
            id = cJSON_GetObjectItem(monitor, "sensor_id");
            value = cJSON_GetObjectItem(monitor, "sensor_voltage");
            timestamp = cJSON_GetObjectItem(monitor, "timestamp");
            Message_t message;
            message.id = (uint8_t)id->valueint;
            message.value = (float)value->valuedouble;
            message.timestamp = (int64_t)timestamp->valuedouble;
            cJSON_Delete(monitor);
            xQueueSendFromISR(xQueue, &message, portMAX_DELAY);
            break;
        case MQTT_EVENT_ERROR:
            // Handle error codes
            ESP_LOGI(TAG, "MQTT Error");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
                ESP_LOGI(TAG, "Last captured errno : %d (%s)", event->error_handle->esp_transport_sock_errno,
                         strerror(event->error_handle->esp_transport_sock_errno));
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        default:
            break;
    }
}

esp_err_t mqtt_handler_initialize(const char* host) {
    esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = host
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_config);
    xQueue = xQueueCreate(150, sizeof(Message_t));
    return esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, event_handler, mqtt_client);
}

esp_err_t mqtt_handler_start(void) {
    return esp_mqtt_client_start(mqtt_client);
}

void mqtt_handler_deinitialize(void) {
    esp_mqtt_client_destroy(mqtt_client);
}

void mqtt_handler_stop(void) {
    esp_mqtt_client_stop(mqtt_client);
}

void mqtt_handler_subscribe(char* topic) {
    esp_mqtt_client_subscribe(mqtt_client, topic, 1);
}
