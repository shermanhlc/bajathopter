#include "CommsInterface.h"

#include "esp_log.h"
#include "esp_wifi.h"

/**
 * This is a wrapper class for the esp_wifi.h api utilizing a Communication Interface to allow for varying library implementations
 */

namespace comms {
    class EspIDFComms : public CommsInterface {
        public:
            EspIDFComms(wifi_config_t wifi_config, wifi_mode_t mode = WIFI_MODE_NULL);
            // EspIDFComms(std::sring& ssid, std::string& password, wifi_mode_t mode = WIFI_MODE_NULL)

            ~EspIDFComms();

            /**
             * @brief Starts the wifi interface based on the type (station/AP)
             * 
             * @return
             *    - ESP_OK: succeed
             *    - ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
             *    - ESP_ERR_INVALID_ARG: It doesn't normally happen, the function called inside the API was passed invalid argument, user should check if the wifi related config is correct
             *    - ESP_ERR_NO_MEM: out of memory
             *    - ESP_ERR_WIFI_CONN: WiFi internal error, station or soft-AP control block wrong
             *    - ESP_FAIL: other WiFi internal errors
             */
            esp_err_t StartWifi() override;

            /**
             * @brief Stops the wifi interface
             * 
             * @return
             *    - ESP_OK: succeed
             *    - ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
             */
            esp_err_t StopWifi() override;

            esp_err_t ConnectToAccessPoint() override;

        private:
            void HandleEvent(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

            wifi_config_t m_wifi_config;
            wifi_mode_t m_mode;
    };
}