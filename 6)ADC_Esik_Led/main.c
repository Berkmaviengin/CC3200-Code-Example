//*****************************************************************************
//
// Application Name     - ADC_Esik_Led
// Application Overview - PIN_15'e bagli led ile PIN_60'a bagli adc degerin
//                        okunarak ledin yakilmasi

//*****************************************************************************

// Standard includes
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Driverlib includes
#include "utils.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_adc.h"
#include "hw_ints.h"
#include "hw_gprcm.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
#include "pinmux.h"
#include "pin.h"
#include "adc.h"
#include "gpio.h"

//#include "adc_userinput.h"
#include "uart_if.h"

#define USER_INPUT 
#define UART_PRINT         Report
#define FOREVER            1
#define APP_NAME           "ADC Reference"
#define NO_OF_SAMPLES 		128

unsigned long pulAdcSamples[4096];

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
static void DisplayBanner(char * AppName);

//*****************************************************************************
//
//! Application startup display on UART
//
//*****************************************************************************
static void DisplayBanner(char * AppName)
{
    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t       CC3200 %s Application       \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//
//*****************************************************************************
static void
BoardInit(void)
{
#ifndef USE_TIRTOS

#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Ýslemci Enable
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}



void main()
{
    unsigned int  uiIndex=0;
    unsigned long ulSample;
    float a;

    //
    // Initialize Board configurations
    //
    BoardInit();

    //
    // Configuring UART for Receiving input and displaying output
    // 1. PinMux setting
    // 2. Initialize UART
    // 3. Displaying Banner
    //
    PinMuxConfig();
    InitTerm();
    DisplayBanner(APP_NAME);

    while(FOREVER)
    {

        uiIndex=0;
      
        //
        // Analog deger okunmasi
        //


#ifdef CC3200_ES_1_2_1
        //
        // Enable ADC clocks.
        //
        HWREG(GPRCM_BASE + GPRCM_O_ADC_CLK_CONFIG) = 0x00000043;
        HWREG(ADC_BASE + ADC_O_ADC_CTRL) = 0x00000004;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE0) = 0x00000100;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE1) = 0x0355AA00;
#endif
        //
        // Input Pininin secilmesi
        //
        MAP_PinTypeADC(PIN_60,PIN_MODE_255);

        //
        // Configure ADC timer
        //
        MAP_ADCTimerConfig(ADC_BASE,2^17);

        //
        // Enable ADC timer
        //
        MAP_ADCTimerEnable(ADC_BASE);

        //
        // Enable ADC module
        //
        MAP_ADCEnable(ADC_BASE);

        //
        // Enable ADC channel
        //

        MAP_ADCChannelEnable(ADC_BASE, ADC_CH_3);

        while(uiIndex < NO_OF_SAMPLES + 4)
        {
            if(MAP_ADCFIFOLvlGet(ADC_BASE, ADC_CH_3))
            {
                ulSample = MAP_ADCFIFORead(ADC_BASE, ADC_CH_3);
                pulAdcSamples[uiIndex++] = ulSample;

            }
        }

        MAP_ADCChannelDisable(ADC_BASE,ADC_CH_3);

        uiIndex = 0;

        while(uiIndex < NO_OF_SAMPLES)
        {
            a=(((float)((pulAdcSamples[4+uiIndex] >> 2 ) & 0x0FFF))*1.4)/4096;
            UART_PRINT("\n\rVoltage is %f\n\r",a);
            uiIndex++;
            if(a<1)
            {
                MAP_GPIOPinWrite(GPIOA2_BASE,GPIO_PIN_6,GPIO_PIN_6);
                MAP_UtilsDelay(8000000);
                break;
            }
            if(a>=1)
            {
                MAP_GPIOPinWrite(GPIOA2_BASE,GPIO_PIN_6,0);
                MAP_UtilsDelay(8000000);
                break;
            }
        }

        UART_PRINT("\n\r");

    }

}

