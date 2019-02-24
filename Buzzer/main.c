
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


void Clock_Init48MHz();
void Initclks();
void InitATimer();
void InitPins();

//global variables
volatile int seconds = 0;

//Timer_A PWM Configuration Parameter
//Sets timer A clk to SMclk divider 1
//This needs to change
//original code M and Sm were set to 128 KHz
Timer_A_PWMConfig pwmConfig1 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        320,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        250
};

//Timer_A PWM Configuration Parameter
//Sets timer A clk to SMclk divider 1
//This needs to change
Timer_A_PWMConfig pwmConfig2 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_32,
        320,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        250
};

int main(void)
0{
    //top Watchdog
    MAP_WDT_A_holdTimer();

    Clock_Init48MHz();      //Sets HFXT to 48 MHz and Mclk to clk divider 1 and SMclk to clk divider 16
    Initclks();             //Sets REFO to 128 KHz and Aclk to clk divider 2
    InitATimer();           //Sets Timer A1 to Aclk, Continuous mode, Clear TAR, Overflow Interrupt

    InitPins();

    // Enable global interrupts
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA1_N_IRQn) & 31); //No idea what this does but was in the code before so I left it

    while(1)
    {
        //Loop indefinitely Like our security system code
    }
}


//---------------------------------------------------------------------------------------------
void Clock_Init48MHz()
{
    /* Configuring pins for peripheral/crystal usage */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
                                                    GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    CS_setExternalClockSourceFrequency(32000,48000000); // enables getMCLK,  getSMCLK to know externally set frequencies
    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false); // false means that there are no timeouts set, will return when stable
    /* Initializing MCLK to HFXT (effectively 48MHz) */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_16);
    // MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    // MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
    //SMCLKfreq=MAP_CS_getSMCLK(); // get SMCLK value to verify it was set to 12 MHz
    //MCLKfreq=MAP_CS_getMCLK(); // get MCLK value and verify it also
}
//---------------------------------------------------------------------------------------------
void Initclks()
{
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);                     //REFO set to 128k Hz
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);     //Set A clock to REFO clock source divider 2
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);     //Set A clock to REFO clock source divider 2
}
//-----------------------------------------------------------------------------------------------
void InitATimer()
{
    // Configure Timer_A 1 this will count to 1.024 Seconds and trigger an overflow interrupt
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_1 |// ACLK
            TIMER_A_CTL_MC_2 |            // Continuous mode
            TIMER_A_CTL_CLR |             // Clear TAR
            TIMER_A_CTL_IE;               // Enable timer overflow interrupt

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;  // Enable sleep on exit from ISR //Not sure what this really means
}
//------------------------------------------------------------------------------------------------
void InitPins()
{
    //Configure GPIO
    P2->SEL0 |= BIT4;                               // Setup the P2.4 to be an output for the alarm.  This should drive a sounder.
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;
    P2->OUT &=~ BIT4;
    //This code should do the same thing as above
//    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,  // Configuring GPIO2.4 as peripheral output for PWM
//            GPIO_PRIMARY_MODULE_FUNCTION);

    P1->DIR |= BIT0;                                //On board red LED
    P1->OUT |= BIT0;

}
//----------------------------------------------------------------------------------------------
// Timer A1_N interrupt service routine
//Initialized in startup_msp432p401r_ccs.c
void TA1_N_IRQHandler(void)
{
    TIMER_A1->CTL &= ~TIMER_A_CTL_IFG;      // Clear timer overflow interrupt flag

    //configure PWM frequency every second
    if((seconds % 2) == 0)
    {
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig1);
    }
    if((seconds % 2) == 1)
    {
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
    }

    seconds++;                              //increment seconds

    P1->OUT ^= BIT0;                        // Toggle P1.0 LED
}







