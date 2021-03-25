#include <stdlib.h>
#include <string.h>

// Driverlib includes
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "uart.h"
#include "interrupt.h"
#include "pinmux.h"
#include "utils.h"
#include "prcm.h"

// Common interface include
#include "uart_if.h"
#include "pin.h"
#include "gpio_if.h"


//*****************************************************************************
//                          MACROS                                  
//*****************************************************************************
#define APPLICATION_VERSION  "1.1.1"
#define APP_NAME             "UART"
#define CONSOLE              UARTA0_BASE
#define UartGetChar()        MAP_UARTCharGet(CONSOLE)
#define UartPutChar(c)       MAP_UARTCharPut(CONSOLE,c)
#define MAX_STRING_LENGTH    80

static void DisplayBanner(char * AppName)
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t        CC3200 %s Uygulamasi      \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}

void getchar()
{

}


void main()
{
    char cCharacter;

    PinMuxConfig(); //rx,tx,led pin ayarlari

    GPIO_IF_LedConfigure(LED1); //pin64 kirmizi led ayarlandi
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);

    InitTerm(); //uart terminal init

    ClearTerm(); //terminal temizlendi

    DisplayBanner(APP_NAME);

    Message("\t\t****************************************************\n\r");
    Message("\t\t\t        CC3200 UART  Kullanimi        \n\r");
    Message("\t\t\t [a] : LED ACIK  \n\r");
    Message("\t\t\t [k] : LED KAPALI  \n\r") ;
    Message("\t\t\t [r] : BASTAN BASLATIR \n\r");
    Message("\t\t\t [y] : YAZAR,YAZILMA TARIHI GOSTERIR \n\r");
    Message("\t\t ****************************************************\n\r");
    Message("\n\n\n\r");
    Message("Karakteri Giriniz >> ");

    while(1)
    {
        cCharacter = UartGetChar(); //gelen karakteri oku

        if(cCharacter=='a') //a gelirse
        {
            Message("\n\r");
            Message("LED ACIK!\n\r");
            GPIO_IF_LedOn(MCU_RED_LED_GPIO); //led yanar
            Message("Karakteri Giriniz >> ");
        }

        if(cCharacter=='k') //k gelirse
        {
            Message("\n\r");
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);  //led soner
            Message("LED KAPALI\n\r");
            Message("Karakteri Giriniz >> ");
        }
        if(cCharacter=='y') // y gelirse
        {
            Message("\n\r");
            Message("\t Yazar : Yunus Berk Maviengin \n\r"); //yazar ve tarih yazar
            Message("\t Yazilma Tarihi : 22.05.2017 \n\r");
            Message("Karakteri Giriniz >> ");
        }

        if(cCharacter=='r') // r gelirse
        {
            return main(); // bastan baslar
        }

        if(!(cCharacter=='a'||cCharacter=='k'||cCharacter=='r'||cCharacter=='y')) //baska karakter gelirse
        {
            Message("\n\r");
            Message("Yanlis Girdi Lutfen Tekrar Deneyin...\n\r"); //tekrar dene der
            Message("Karakteri Giriniz >> ");
        }




    }
}

