/*
 * SPI.c
 *
 *  Created on: Feb 4, 2019
 *      Author: andre
 */

#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include "utringbuffer.h"
#include <string.h>
#include <SPI.h>

int RXData;
int TXData;

void SPI_Init(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
    WDT_A_CTL_HOLD;

    P1->OUT &= ~BIT0;
    P1->DIR |= BIT0;                        // Set P1.0 LED

    P2->DIR |= BIT5;                        //CS Bit

    P2->OUT &=~BIT5;
    P1->SEL0 |= BIT5 | BIT6 | BIT7;         // Set P1.5, P1.6, and P1.7 as
                                            // SPI pins functionality

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST | // Remain eUSCI state machine in reset
    EUSCI_B_CTLW0_MST |             // Set as SPI master
    EUSCI_B_CTLW0_SYNC |            // Set as synchronous mode
    EUSCI_B_CTLW0_CKPL |            // Set clock polarity high
    EUSCI_B_CTLW0_MSB;              // MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__ACLK; // ACLK
    EUSCI_B0->BRW = 0x01;                   // /2,fBitClock = fBRCLK/(UCBRx+1).
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;// Initialize USCI state machine

    __enable_irq();


    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
    //SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
}

void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)
    {
        EUSCI_B0->TXBUF = TXData;           // Transmit characters
        EUSCI_B0->IE &= ~EUSCI_B__TXIE;
        // Wait till a character is received
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
        RXData = EUSCI_B0->RXBUF;
        // Clear the receive interrupt flag
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG;
    }
}


void SPI_WriteLatch(void)
{
    TXData=0x6;
}
void SPI_Write(WriteBuffer *buff1)
{
    uint8_t i;
    //Operational Code for write.
    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
    EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
    TXData=0x2
    //Transmitting the address on flash chip.
    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
    EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
    TXData=(buff1->address & 0xFF00)>>8;
    TXData=(buff1->address & 0x00FF);
    //Transmitting actual data to the specific address.

    for(i=0; i<buff1->size; i++)
        EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
        EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
        TXData=buff1->data[i];
    }
}

void SPI_Read(void)
{
    TXData=0x3
}

void SPI_WriteUnlatch(void)
{
    TXData=0x4
}



