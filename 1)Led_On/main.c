#include "pin_mux_config.h"
// Standard includes
#include <stdio.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Common interface includes
#include "gpio_if.h"
int main(void)
{
   //  IntMasterEnable();   // interapt ac
   // IntEnable();         // bu da �nterapt dalgas�
    PRCMCC3200MCUInit();  // bord baslang�c ayarlar�
    PinMuxConfig();     // giri� ��k�s ayarlar�

    while(1)
    {
        GPIOPinWrite(GPIOA1_BASE, 0x1, 1);
        UtilsDelay(10000);
        GPIOPinWrite(GPIOA1_BASE, 0x1, 0);
        UtilsDelay(10000);

    }
}
