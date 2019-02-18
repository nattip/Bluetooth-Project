/*
 * UART.c
 *
 *  Created on: Jan 9, 2019
 *  Author: Andrew Deradoorian and Natalie Tipton
 */


#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include <string.h>
#include <SPI.h>
#include <Flash.h>


char readData;

void UART0_Init(void)
{
    EUSCI_A0->CTLW0 |=1;    //put in reset mode
    EUSCI_A0->MCTLW=0;      //disable oversampling
    EUSCI_A0->CTLW0=0x0081; //1 stop bit, no parity, SMCLK, 8 bit data
    EUSCI_A0->BRW=26;       //3000000 / 115200 = 26
    P1->SEL0 |=0x0C;        //P1.3, P1.2 set up for UART
    P1->SEL1 &=~ 0x0C;

    EUSCI_A0->CTLW0 &=~ 1; //End config of EUSCI
    EUSCI_A0->IE |= 0x01; //Interrupt enable

    NVIC_SetPriority(EUSCIA0_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}
void UART2_Init(void)
{
    EUSCI_A2->CTLW0 |=1;    //put in reset mode
    EUSCI_A2->MCTLW=0;      //disable oversampling
    EUSCI_A2->CTLW0=0x0081; //1 stop bit, no parity, SMCLK, 8 bit data
    EUSCI_A2->BRW=26;       //3000000 / 115200 = 26
    P3->SEL0 |=0x0C;        //P3.3, P3.2 set up for UART
    P3->SEL1 &=~ 0x0C;

    EUSCI_A2->CTLW0 &=~ 1;  //take UART out of reset mode
    EUSCI_A2->IE |= 0x01; //Interrupt enable

    NVIC_SetPriority(EUSCIA2_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}


char UART0_Read(void)
{
    while(!(EUSCI_A0->IFG & 0x01)) {}   //wait until UART receive buffer is full
    readData = EUSCI_A0->RXBUF; //save the read data into a buffer
    return readData;    //return the read data back to main
}

char UART2_Read(void)
{
    while(!(EUSCI_A2->IFG & 0x01)) {}   //wait until UART receive buffer is full
    readData = EUSCI_A2->RXBUF; //save the read data into a buffer
    return readData;    //return the read data back to main
}

void UART0_Write(readPoem* sendPoem)
{
    uint16_t i;
    //sendPoem->data[sendPoem->size+1]="*";
    for(i=0;i<((sendPoem->size)+1);i++)
    {
    while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
    EUSCI_A0->TXBUF=sendPoem->data[i];   //write the new speed into the buffer
    }
}

void UART0_WriteIndex(readPoem* sendPoem)
{
    uint16_t i;
    uint8_t jokes;
    jokes=strlen(sendPoem->data);
    for(i=0;i<jokes;i++)
    {
    while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
    EUSCI_A0->TXBUF=sendPoem->data[i];   //write the new speed into the buffer
    }
}


void UART2_Write(readPoem* sendPoem)
{
    uint16_t i;
    //sendPoem->data[sendPoem->size+1]="*";
    for(i=0;i<((sendPoem->size)+1);i++)
    {
    while(!(EUSCI_A2->IFG & 0x02)) {}   //wait until UART send suffer is full
    EUSCI_A2->TXBUF=sendPoem->data[i];   //write the new speed into the buffer
    }
}

void UART2_WriteIndex(readPoem* sendPoem)
{
    uint16_t i;
    uint8_t jokes;
    jokes=strlen(sendPoem->data);
    for(i=0;i<jokes;i++)
    {
    while(!(EUSCI_A2->IFG & 0x02)) {}   //wait until UART send suffer is full
    EUSCI_A2->TXBUF=sendPoem->data[i];   //write the new speed into the buffer
    }
}

