#ifndef FART_H
#define FART_H

#include <stdio.h>
// #include "esp_wifi.h"

#include "esp_log.h"
#include "nvs_flash.h"
/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
// #include "blecent.h"

void hello(void);


#endif // FART_H