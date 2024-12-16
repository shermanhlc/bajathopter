#include "mqtt_handler.h"
#include <stdbool.h>
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"

#define TAG "MQTT"

#define TOPIC "/topic/sensors"

static esp_mqtt_client_handle_t mqtt_client;

static void event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT Disconnected");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT Published");
            break;
        case MQTT_EVENT_ERROR:
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

esp_err_t mqtt_handler_publish(uint8_t id, float value, int64_t timestamp) {
    // Format data to JSON and publish
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "sensor_id", id);
    cJSON_AddNumberToObject(json, "sensor_voltage", value);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    char *json_str = cJSON_Print(json);
    esp_mqtt_client_publish(mqtt_client, TOPIC, json_str, strlen(json_str), 1, false);
    return ESP_OK;
}