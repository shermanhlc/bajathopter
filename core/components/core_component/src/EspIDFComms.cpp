#include "EspIDFComms.h"

namespace comms {
    EspIDFComms::EspIDFComms(wifi_config_t wifi_config, wifi_mode_t mode) 
    {
        m_wifi_config = wifi_config;

        // init the network interface
        ESP_ERROR_CHECK(esp_netif_init());

        // start default event loop
        ESP_ERROR_CHECK(esp_event_loop_create_default());

        if(mode == WIFI_MODE_STA) 
        {
            esp_netif_create_default_wifi_sta();
            m_mode = mode;
        }
        else if(mode == WIFI_MODE_AP)
        {
            esp_netif_create_default_wifi_ap();
            m_mode = mode;
        }
        else
        {
            // defaults to station mode, but this could be swapped to APSTA and handle so it could act as either
            ESP_LOGE(COMMS_TAG, "Use WIFI_MODE_STATION or WIFI_MODE_AP for communication mode, defaulting to WIFI_MODE_STA");
            esp_netif_create_default_wifi_sta();
            m_mode = WIFI_MODE_STA;
        }

        wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&initConfig));

        esp_event_handler_instance_t anyId;
        esp_event_handler_instance_t gotIp;
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &EspIDFComms::HandleEvent, NULL, &anyId));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &EspIDFComms::HandleEvent, NULL, &gotIp));f
    }

    esp_err_t EspIDFComms::startWifi() 
    {
        return ESP_ERROR_CHECK(esp_wifi_start());
    }

    esp_err_t EspIDFComms::stopWifi() 
    {
        return ESP_ERROR_CHECK(esp_wifi_stop());
    }

    esp_err_t EspIDFComms::connectToAccessPoint()
    {

    }

    void EspIDFComms HandleEvent()
}