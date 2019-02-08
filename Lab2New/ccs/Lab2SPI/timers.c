/*
 * timers.c
 *
 *  Created on: Jan 9, 2019
 *  Author: Andrew Deradoorian and Natalie Tipton
 */


#include "msp.h"
#include <stdio.h>
#include <timers.h>
#include <UART.h>
#include <IO.h>


void timerA_Init()
{
    TIMER_A1->CTL= 0x01D1;      //Timer overflowed, interrupt disable, timer cleared, up mode, clock divide by 8, ACLK
    TIMER_A1->EX0=0;            //External divider disabled
    TIMER_A1->CCR[0]=4096-1;    //top value
    TIMER_A1->CCTL[0]|=0x10;    //enable TimerA interrupts
    __enable_irq();             //enable interrupts
}

void TA1_0_IRQHandler (void)
{
    TIMER_A1->CCTL[0] &=~1; //reset interrupt flag
    P2->OUT ^=1;    //toggle onboard LED
}
