/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_21_00_05 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 ADC14 - Single Channel Sample Repeat
 *
 * Description: This code example will demonstrate the basic functionality of
 * of the DriverLib ADC APIs with sampling a single channel repeatedly. Each
 * time the ADC conversion occurs, the result is stored into a local variable.
 * The sample timer is used to continuously grab a new value from the ADC
 * module using a manual iteration that is performed in the ADC ISR. A
 * normalized ADC value with respect to the 3.3v Avcc is also calculated using
 * the FPU.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P5.5  |<--- A0 (Analog Input)
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * Author: Timothy Logan
 ******************************************************************************/

/****************************************
 * Written by Natalie Tipton
 * EGR 326 (902)
 * Dr. Nabeeh Kandalaft
 * Lab4_Prelab
 * This code returns the digital value of
 *     an analog input from a photcell sensor
 * 9/21/17
 ****************************************/

#include "driverlib.h"                                                  // DriverLib Includes
#include <stdint.h>                                                     // Standard Includes
#include <stdbool.h>

static volatile uint16_t curADCResult;                                  // Statics
static volatile float normalizedADCRes;

void SysTick_Init(void);
void SysTick_delay(uint16_t delay);

int main(void)
{
    MAP_WDT_A_holdTimer();                                                  // Halting the Watchdog
    SysTick_Init();

    curADCResult = 0;                                                   // Initializing Variables

    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);                      // Setting Flash wait state
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);                           // Setting DCO to 48MHz
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    
    MAP_FPU_enableModule();                                         // Enabling the FPU for floating point operation
    MAP_FPU_enableLazyStacking();

    MAP_ADC14_enableModule();                                                               // Initializing ADC (MCLK/1/4)
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0);
            
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,                 // Configuring GPIOs (5.5 A0)
    GPIO_TERTIARY_MODULE_FUNCTION);

    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);                                    // Configuring ADC Memory
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A0, false);

    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);                                  // Configuring Sample Timer

    MAP_ADC14_enableConversion();                                                   // Enabling/Toggling Conversion
    MAP_ADC14_toggleConversionTrigger();

    MAP_ADC14_enableInterrupt(ADC_INT0);                                            // Enabling interrupts
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    while (1)
    {
        MAP_PCM_gotoLPM0();

    }
    
}

/************************************************
 *  Written by Timothy Logan
 *  Implemented by Natalie Tipton
 *  This coded is an ADC Interrupt Handler
 *      that is called whenever there is a
 *      conversion that is finished for ADC_MEM0
 ************************************************/

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT0 & status)
    {
        SysTick_delay(5000);
        curADCResult = MAP_ADC14_getResult(ADC_MEM0);
        normalizedADCRes = (curADCResult * 3.3) / 16384;

        printf("%d\n",curADCResult);

        MAP_ADC14_toggleConversionTrigger();
    }
}

/******************************************
 * Initialize SysTick Timer
 * Taken from EGR 326 lecture 3
 * Lecture by Dr. Bossemeyer
 * Implemented by Natalie Tipton
 * 9/7/17
 ******************************************/

void SysTick_Init(void)
{
    SysTick -> CTRL = 0;                            // disable SysTick during setup
    SysTick -> LOAD = 0x00FFFFFF;                   // maximum reload value
    SysTick -> VAL = 0;                             // any write to current value clears it
    SysTick -> CTRL = 0x00000005;                   // enable SysTick, CPU clk, no interrupts
}

/******************************************
 * Create delay using SysTick Timer
 * Taken from EGR 326 lecture 3
 * Lecture by Dr. Bossemeyer
 * Implemented by Natalie Tipton
 * 9/7/17
 ******************************************/

void SysTick_delay(uint16_t delay)
{
    SysTick -> LOAD = ((delay*3000) - 1);           // 1ms count down to zero
    SysTick -> VAL = 0;                             // any write to CVR clears it
                                                    // and COUNTFLAG in CSR

    while((SysTick -> CTRL & 0x00010000) == 0);     // Wait for flag to be SET (Timeout happened)
}

