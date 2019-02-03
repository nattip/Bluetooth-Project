/****************************************
 * Written by Natalie Tipton and Andrew Deradoorian
 * EGR 436 (102)
 * Dr. Brakora
 * Lab 1
 * 1/7/19
 * This code uses interrupts to flash an onboard red
 *      LED at specific time intervals. These intervals
 *      are dependent on data that is read in from the
 *      computer over UART.
 ****************************************/

#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>

uint16_t changedSpeed=60;   //speed of flashing LED


void main(void)
{
    __disable_irq();    //disable interrupts to start

    //initialize variables
    uint16_t TopValue=4096;
    uint16_t basespeed=60;
    uint16_t ratio;
    uint16_t AlteredCount;
    char readData;


    STOP_WDT;   //stop watch dog timer

    NVIC_SetPriority(TA1_0_IRQn,3);     //set priority of interrupt
    NVIC_EnableIRQ(TA1_0_IRQn);         //enable interrupt

    UART0_Init();   //initialize UART0
    timerA_Init();  //initialize TimerA
    onboardLED_setup(); //setup onboard LED

    //continuous while loop
    while(1){

       ratio=(100*basespeed)/changedSpeed; //math for the ratio between basespeed and the altered speed
       AlteredCount=(TopValue*ratio)/100; //altering the top count value to be scaled to correct bpm
       TIMER_A1->CCR[0]=AlteredCount/2; //setting top count value in register

       readData = UART0_Read(); //reads data from UART

       if(readData == 'u') changedSpeed += 2;   //if a 'u' is read in from UART, increase speed by 2 bpm
       else if(readData == 'd') changedSpeed -= 2;  //if a 'd' is read in from UART, decrease speed by 2 bmp
       else if(readData == 'r') changedSpeed = 60;  //if an 'r' is read in from UART, reset speed to 60 bpm

       UART0_Write(changedSpeed);   //writes current speed over UART to the computer
    }
}

