#include "led.h"

#include "led_strip.h"
#include "esp_log.h"

#define BLINK_GPIO 6

static uint8_t led_state = 0;

static led_strip_handle_t led_strip;

static const char *TAG = "LED";

void blink_led(void)
{
    /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
    led_strip_set_pixel(led_strip, 0, 0, 6, 255);
    /* Refresh the strip to send data */
    led_strip_refresh(led_strip);
}

void clear_led(void) {
    led_strip_clear(led_strip);
}

void init_led(void) {
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");

    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, //10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    led_strip_clear(led_strip);
}