#include <stdio.h>

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "utils.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio_if.h"
#include "pin_mux_config.h"

#define APPLICATION_VERSION     "1.1.1"

int main()
    {
    //**** Board Yapilandirmalari ****//
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    PRCMCC3200MCUInit();
    //******************************//

    PinMuxConfig();     // Pin ayarlamalari yapildi

    while(1)
    {
        MAP_GPIOPinWrite(GPIOA1_BASE, 0x1,1);       //P63 pini yani GPIO00 lojik 1 yapildi (Mavi led)
        MAP_GPIOPinWrite(GPIOA0_BASE, 0x1,0);       //P50 pini yani GPIO00 lojik 0 yapildi (Mavi led)
        MAP_UtilsDelay(8000000);
        MAP_GPIOPinWrite(GPIOA1_BASE, 0x1,0);       //P63 pini yani GPIO00 lojik 0 yapildi (Mavi led)
        MAP_GPIOPinWrite(GPIOA0_BASE, 0x1,1);       //P50 pini yani GPIO00 lojik 1 yapildi (Mavi led)
        MAP_UtilsDelay(8000000);


    }
}
