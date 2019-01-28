/*
 * timers.c
 *
 *  Created on: Jan 9, 2019
 *      Author: andrew
 */


#include "msp.h"
#include <stdio.h>
#include <timersinit.h>




void timerA_Init()
{
    TIMER_A1->CTL= 0x01D1;
    TIMER_A1->EX0=0;
  //  TIMER_A1->CCR[0]=4096-1;
    TIMER_A1->CCTL[0]|=0x10;
    __enable_irq();


}
