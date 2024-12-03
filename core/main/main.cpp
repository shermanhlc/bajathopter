#define TAG "MAIN"

// #include "esp_log.h"x
#include "fart.h"
#include "EspIDFComms.h"

using namespace comms;

extern "C" void app_main() 
{
    hello();
    wifi_config_t wc;
    EspIDFComms eic = EspIDFComms(wc, WIFI_MODE_AP);
}