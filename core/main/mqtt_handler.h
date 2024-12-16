#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "esp_err.h"


esp_err_t mqtt_handler_initialize(const char* host);

esp_err_t mqtt_handler_start(void);

esp_err_t mqtt_handler_publish(uint8_t id, float value, int64_t timestamp);

void mqtt_handler_deinitialize(void);

void mqtt_handler_stop(void);

#endif /* MQTT_HANDLER_H */