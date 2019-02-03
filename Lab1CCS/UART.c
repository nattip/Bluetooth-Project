/*
 * UART.c
 *
 *  Created on: Jan 9, 2019
 *  Author: Andrew Deradoorian and Natalie Tipton
 */


#include "msp.h"
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>

char readData;

void UART0_Init(void)
{
    EUSCI_A0->CTLW0 |=1;    //put in reset mode
    EUSCI_A0->MCTLW=0;      //disable oversampling
    EUSCI_A0->CTLW0=0x0081; //1 stop bit, no parity, SMCLK, 8 bit data
    EUSCI_A0->BRW=26;       //3000000 / 115200 = 26
    P1->SEL0 |=0x0C;        //P1.3, P1.2 set up for UART
    P1->SEL1 &=~ 0x0C;
    EUSCI_A0->CTLW0 &=~ 1;  //take UART out of reset mode
}

char UART0_Read(void)
{
    while(!(EUSCI_A0->IFG & 0x01)) {}   //wait until UART receive buffer is full
    readData = EUSCI_A0->RXBUF; //save the read data into a buffer
    return readData;    //return the read data back to main
}

void UART0_Write(uint16_t changedSpeed)
{
    while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
    EUSCI_A0->TXBUF=changedSpeed;   //write the new speed into the buffer
}

