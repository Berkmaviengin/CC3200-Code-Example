//*****************************************************************************
//
//  Lab 1
//
//  Team member 1: Jau Shiuan Shiao
//  Team member 2: Teja Aluru
//
//*****************************************************************************

//*****************************************************************************
//
// Application Name     - Binary counter and Blinky in unison
//
//*****************************************************************************

//****************************************************************************
//
//! \addtogroup blinky
//! @{
//
//****************************************************************************

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

// Driverlib includes
#include "uart.h"
// Common interface include
#include "uart_if.h"

// Common interface includes
#include "gpio_if.h"

#include "pin_mux_config.h"

#define APPLICATION_VERSION     "1.1.1"
#define APP_NAME             "GPIO"
#define LED_RED 0x2
#define LED_YELLOW 0x4
#define LED_GREEN 0x8

#define Initial 0
#define SW3Pressed 1
#define SW2Pressed 2

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************
long value; // // current output value for LEDs

//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//*****************************************************************************
void SW3Routine();
void SW2Routine();

static void BoardInit(void);

static void
DisplayBanner(char * AppName)
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t        CC3200 %s Application       \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}






//*****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS
//*****************************************************************************

//*****************************************************************************
//
//! Configures the pins as GPIOs and peroidically toggles the lines
//!
//! \param None
//!
//! This function
//!    1. Configures 3 lines connected to LEDs as GPIO
//!    2. Sets up the GPIO pins as output
//!    3. Periodically toggles each LED one by one by toggling the GPIO line
//!
//! \return None
//
//*****************************************************************************
void SW3Routine(){
    if(value == 0x0){
        GPIOPinWrite(GPIOA1_BASE, 0x2|0x4|0x8, 0);  // 000
        MAP_UtilsDelay(4000000);
        value = LED_RED;
    }else if(value == LED_RED){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);        // 001
        MAP_UtilsDelay(4000000);
        value = LED_YELLOW;
    }else if(value == LED_YELLOW){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0);          // 010
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
        MAP_UtilsDelay(4000000);
        value = (LED_RED|LED_YELLOW);
    }else if(value == (LED_RED|LED_YELLOW)){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);        // 011
        MAP_UtilsDelay(4000000);
        value = LED_GREEN;
    }else if(value == LED_GREEN){
        GPIOPinWrite(GPIOA1_BASE, 0x2|0x4, 0);      // 100
        GPIOPinWrite(GPIOA1_BASE, 0x8, 0x8);
        MAP_UtilsDelay(4000000);
        value = (LED_GREEN|LED_RED);
    }else if(value == (LED_GREEN|LED_RED)){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);        // 101
        MAP_UtilsDelay(4000000);
        value = (LED_GREEN|LED_YELLOW);
    }else if(value == (LED_GREEN|LED_YELLOW)){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0);          // 110
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
        MAP_UtilsDelay(4000000);
        value = (LED_GREEN|LED_YELLOW|LED_RED);
    }else if(value == (LED_GREEN|LED_YELLOW|LED_RED)){
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);        // 111
        MAP_UtilsDelay(4000000);
        value = 0x0;
    }
}

void SW2Routine(){
    if(value == 0x0){
        GPIOPinWrite(GPIOA1_BASE, 0x2|0x4|0x8, 0);  // 000
        MAP_UtilsDelay(3000000);
        value = (LED_RED|LED_YELLOW|LED_GREEN);
    }
    else if(value == (LED_RED|LED_YELLOW|LED_GREEN) ){
        GPIOPinWrite(GPIOA1_BASE, 0x2|0x4|0x8, 0x2|0x4|0x8);  // 111
        MAP_UtilsDelay(3000000);
        value = 0x0;
    }
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif

    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}
//****************************************************************************
//
//! Main function
//!
//! \param none
//!
//! This function
//!    1. Invokes the LEDBlinkyTask
//!
//! \return None.
//
//****************************************************************************
int
main()
{
    //
    // Initialize Board configurations
    //
    BoardInit();

    //
    // Set up the GPIO
    //
    PinMuxConfig();

    // Initialize the Terminal
    InitTerm();
    //
    // Clearing the Terminal.
    //
    ClearTerm();
    //
    GPIO_IF_LedConfigure(LED1|LED2|LED3);

    GPIO_IF_LedOff(MCU_ALL_LED_IND);


    long pinVal_1=0;    // variable holding the pinRead from SW3
    long pinVal_2=0;    // variable holding the pinRead from SW2
    value = 0x0;    // current output value for LEDs

    DisplayBanner(APP_NAME);

    int State = Initial;
    GPIOPinWrite(GPIOA1_BASE, 0x2|0x4|0x8, 0);

    while(1){

        pinVal_1 = GPIOPinRead(GPIOA1_BASE, 0x20);  //SW3
        pinVal_2 = GPIOPinRead(GPIOA2_BASE, 0x40);  //SW2

        switch(State){
            case Initial:
                Message("\t In while loop \n\r") ;
                MAP_UtilsDelay(3000000);
                if((pinVal_1 & 0x20)!=0){   // enter SW3 State
                    value = 0x0;
                    GPIOPinWrite(GPIOA3_BASE, 0x10, 0);     // P18 low
                    Message("\t SW3 pressed \n\r") ;
                    State = SW3Pressed;
                }
                else if((pinVal_2 & 0x40)!=0){   // enter SW2 State
                    value = 0x0;
                    GPIOPinWrite(GPIOA3_BASE, 0x10, 0x10);  // P18 high
                    Message("\t SW2 pressed \n\r") ;
                    State = SW2Pressed;
                }
                break;
            case SW3Pressed:                // Previous State is SW3
                if((pinVal_2 & 0x40)!=0){   // SW2 Pressed
                    value = 0x0;
                    GPIOPinWrite(GPIOA3_BASE, 0x10, 0x10);  // P18 high
                    Message("\t SW2 pressed \n\r") ;
                    State = SW2Pressed;
                }else{                      // Do the SW3 routine
                    SW3Routine();
                }
                break;
            case SW2Pressed:                // Previous State is SW2
                if((pinVal_1 & 0x20)!=0){   // SW3 Pressed
                    value = 0x0;
                    GPIOPinWrite(GPIOA3_BASE, 0x10, 0);    // P18 low
                    Message("\t SW3 pressed \n\r") ;
                    State = SW3Pressed;
                }else{                      // Do the SW2 routine
                    SW2Routine();
                }
                break;
            default:
                break;
        }

    }


}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
