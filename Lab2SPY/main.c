#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include "utringbuffer.h"
#include <string.h>
#include <SPI.h>


/**
 * main.c
 */

extern int RXData;
extern int TXData;


void main(void)
{

    int i;
    uint8_t previousData;


    SPI_Init();
    while(1)
       {


           EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
           EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
           TXData = 0x9F;
           // Check the received data
           previousData = TXData - 1;

           if (RXData != (previousData))
           {
               // If the Received data is not equal to TXData-1, then
               // Set P1.0 LED
               P1->OUT |= BIT0;
           }
           else
           {
               P1->OUT &= ~BIT0;
           }
           for (i = 2000; i > 0; i--);         // Delay before next transmission
           TXData=0x9F;                           // Increment transmit data
       }
}


