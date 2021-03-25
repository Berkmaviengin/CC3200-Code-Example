/*
 *      Parlak olan entegre TMP006 infirared Sicaklik sensorudur.
 *      Bu uygulamada Fahrenheit degerinden uarta degerler yazilmistir.
 *      sw2 butonuna basildiginda sicakliklar okunur
 *      Onemli not elini sensore dokundurma deger sapitiyor.
 */

/*
 *      Yazar: Yunus Berk Maviengin
 *      Tarih: 18.05.2017
 */

// Standard includes
#include <stdlib.h>
#include <string.h>

// Driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "utils.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "pin.h"

// Common interface includes
#include "gpio_if.h"
#include "uart_if.h"
#include "i2c_if.h"

// App Includes
#include "tmp006drv.h"
#include "pinmux.h"

#define UART_PRINT         Report
#define FOREVER            1
#define APP_NAME           "ADC_UART"
#define APP_NAME           "TMP006 UART"



//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

/****************************************************************************/
/*                      LOCAL FUNCTION PROTOTYPES                           */
/****************************************************************************/
static void BoardInit(void);

static void
DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t           CC3200 %s Programi       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}


static void BoardInit(void)
{

#ifndef USE_TIRTOS

#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif  //ccs
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif  //ewarm

#endif  //USE_TIRTOS

    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

void  SW2Routine()
{
    while(1)
    {
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        float Sicaklik ;
        TMP006DrvGetTemp(&Sicaklik);
        UART_PRINT("    Sicaklik(F): %f \n\r", Sicaklik);
        UART_PRINT("-------------------------------\n\r");
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
        MAP_UtilsDelay(15000000);
    }


}

void main()
{
    long pinVal=0;    // variable holding the pinRead from SW2

    BoardInit(); //launchpad init
    PinMuxConfig(); //pinmux init
    GPIO_IF_LedConfigure(LED1); //pin64 kirmizi led ayarlandi

    InitTerm(); //uart init

    I2C_IF_Open(I2C_MASTER_MODE_FST); //i2c init
    TMP006DrvOpen(); //tmp006 aktif

    DisplayBanner(APP_NAME);

    UART_PRINT("  Sicaklik Okumak Icin Lutfen SW2 Butonuna basiniz !!");

    while(FOREVER)
    {
        pinVal = GPIOPinRead(GPIOA2_BASE, 0x40);  //SW2 butonunu oku

        if((pinVal & 0x40)!=0) //Eger butona basilirsa
        {
            UART_PRINT("\n\n\r");
            UART_PRINT("  [Butona Basildi]\n\r");
            MAP_UtilsDelay(10000000);
            UART_PRINT("\n\n\r");
            UART_PRINT("-------------------------------\n\r");
            SW2Routine(); //sonsuz sicaklik
        }




    }

}

