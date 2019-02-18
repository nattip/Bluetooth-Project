/*
 * IO.c
 *
 *  Created on: Jan 9, 2019
 *  Author: Andrew Deradoorian and Natalie Tipton
 */


#include "msp.h"
#include <stdio.h>
#include <IO.h>
#include <timers.h>
#include <UART.h>



void onboardLED_setup(void)
{
    P2->SEL1 &=~1; //Setting pin as general IO
    P2->SEL0 &=~1;
    P2->DIR |=1; //Setting data direction as output
}


