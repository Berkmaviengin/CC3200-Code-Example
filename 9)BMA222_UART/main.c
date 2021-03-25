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
#include <stdint.h>


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
#include "bma222drv.h"
#include "pinmux.h"

#define UART_PRINT         Report
#define FOREVER            1
#define APP_NAME           "ADC_UART"
#define APP_NAME           "BMA222 UART"


static void DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t           CC3200 %s Programi       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

void  SW2Routine() //BMA22 okuma
{
    signed char x,y,z;

    while(FOREVER)
    {
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        BMA222ReadNew(&x, &y, &z);
        UART_PRINT("    x-axis: %d \n\r", x);
        UART_PRINT("    y-axis: %d \n\r", y);
        UART_PRINT("    z-axis: %d \n\r", z);
        UART_PRINT("-------------------------------\n\r");
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
        MAP_UtilsDelay((90*80*1000)); //30msec
    }
}

void main()
{

    long pinVal=0;    // variable holding the pinRead from SW2

    PinMuxConfig(); //pinmux init
    GPIO_IF_LedConfigure(LED1); //pin64 kirmizi led ayarlandi

    InitTerm(); //uart init //Terminal init

    I2C_IF_Open(I2C_MASTER_MODE_FST); //i2c init
    BMA222Open(); //BMA222 aktif

    DisplayBanner(APP_NAME);

    UART_PRINT("  Axisleri Okumak Icin Lutfen SW2 Butonuna basiniz !!");

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
            SW2Routine(); //sonsuz axis
        }




    }

}

