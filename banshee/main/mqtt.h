#ifndef MQTT_H
#define MQTT_H

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>

typedef struct {
    uint8_t id;
    float value;
    int64_t timestamp;
} Message_t;

esp_err_t mqtt_handler_initialize(const char* host);

esp_err_t mqtt_handler_start(void);

void mqtt_handler_deinitialize(void);

void mqtt_handler_stop(void);

void mqtt_handler_subscribe(char* topic);

extern QueueHandle_t xQueue;

#endif /* MQTT_H */