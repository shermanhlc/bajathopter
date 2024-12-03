#include <string>

#include "esp_err.h"

/* 
what should this interface have?
start/stop
connect to BEACON
set sleep modes (use an enum, scream if cant)
    - none
    - min (moderate)
    - max
event handling (both station and AP modes)
    - start
    - disconnected
    - connected
set mode (station/AP)
*/

namespace comms {
    #define COMMS_TAG "COMMS"

    enum class PowerMode {
        NONE,
        MIN,
        MAX
    };

    enum class Mode {
        STATION,
        AP,
        BOTH
    };

    class CommsInterface {
        public:
            virtual esp_err_t StartWifi()=0;
            virtual esp_err_t StopWifi()=0;

            virtual esp_err_t ConnectToAccessPoint()=0;

            virtual void SetPowerSavingMode(PowerMode mode)=0;
            virtual void SetMode(Mode mode)=0;
    };
}
