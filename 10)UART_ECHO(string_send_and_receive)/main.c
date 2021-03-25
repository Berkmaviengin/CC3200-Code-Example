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

//*****************************************************************************
//                          MACROS                                  
//*****************************************************************************
#define APPLICATION_VERSION  "1.1.1"
#define APP_NAME             "UART Eko"
#define CONSOLE              UARTA0_BASE
#define UartGetChar()        MAP_UARTCharGet(CONSOLE)
#define UartPutChar(c)       MAP_UARTCharPut(CONSOLE,c)
#define MAX_STRING_LENGTH    80

volatile int g_iCounter = 0;

static void DisplayBanner(char * AppName)
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t        CC3200 %s Uygulamasi      \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}

void main()
{
    char cString[MAX_STRING_LENGTH+1];
    char cCharacter;
    int iStringLength = 0;

    PinMuxConfig(); //rx,tx pin ayarlari

    InitTerm(); //uart terminal init

    ClearTerm(); //terminal temizlendi

    DisplayBanner(APP_NAME);

    Message("\t\t****************************************************\n\r");
    Message("\t\t\t        CC3200 UART Eko Kullanimi        \n\r");
    Message("\t\t      Yazilan veri RX bacagina gider.  \n\r");
    Message("\t\t      Gelen veri terminale yazilir \n\r") ;
    Message("\t\t      Not: maksimum 80 karakter olabilir. \n\r");
    Message("\t\t      Yazdiktan sonra ENTER tusuna basin \n\r");
    Message("\t\t ****************************************************\n\r");
    Message("\n\n\n\r");
    Message("CMD# ");

    while(1)
    {
        cCharacter = UartGetChar(); //gelen karakteri oku
        g_iCounter++; //sayaci bir arttir

        if(cCharacter == '\r' || cCharacter == '\n' || (iStringLength >= MAX_STRING_LENGTH -1)) //entera basilirsa
        {
            if(iStringLength >= MAX_STRING_LENGTH - 1) //butun karakterleri yazma
            {
                UartPutChar(cCharacter);
                cString[iStringLength] = cCharacter;
                iStringLength++;
            }
            cString[iStringLength] = '\0';
            iStringLength = 0;// sifirla

            Report("\n\rCMD# %s\n\r\n\rCMD# ", cString);
        }
        else
        {
            UartPutChar(cCharacter);
            cString[iStringLength] = cCharacter;
            iStringLength++;
        }
    }
}
