#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "esp_err.h"

typedef struct {
    char* host;
    char* topic;
} mqtt_handler_config_t;

esp_err_t mqtt_handler_initialize(mqtt_handler_config_t mqtt_config);

esp_err_t mqtt_handler_start(void);

esp_err_t mqtt_handler_publish(uint8_t id, float value, int64_t timestamp);

void mqtt_handler_deinitialize(void);

void mqtt_handler_stop(void);

#endif /* MQTT_HANDLER_H */