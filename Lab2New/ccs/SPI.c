/*
 * SPI.c
 *
 *  Created on: Feb 4, 2019
 *      Author: andrew
 */

#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include "utringbuffer.h"
#include <string.h>
#include <SPI.h>
#include <Flash.h>


uint8_t RXData;
uint8_t TXData;
uint8_t SPIFlag;
uint8_t SPIFlag2;
uint8_t j;
char helpme[100]={0};
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

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // ACLK
    EUSCI_B0->BRW = 0x01;                   // /2,fBitClock = fBRCLK/(UCBRx+1).
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;// Initialize USCI state machine



   EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
   EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag

//    TXData=00;
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);


}

//IRQ for spi euscib0
void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)
    {
        SPIFlag=1;
        EUSCI_B0->IFG &=~EUSCI_B_IFG_TXIFG;
    }
    if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG)
    {
        SPIFlag2=1;
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG|EUSCI_B_IFG_TXIFG);

    }

}
/*
SPI_WriteLatch sends the write latch command to the flash device. Allowing the user to write to the device.
*/
void SPI_WriteLatch(void)
{
    CSLow;

    EUSCI_B0->TXBUF = 0b00000110;
    while(!SPIFlag);
    SPIFlag=0;

    CSHigh;
}
/*
SPI_WriteUnlatch, not used because toggling the CS after a write related command also unlatches it.
*/
void SPI_WriteUnlatch(void)
{
    CSLow;

    EUSCI_B0->TXBUF = 0b00000100;
    while(!SPIFlag);
    SPIFlag=0;

    CSHigh;
}
/*
SPI_Write sends the write command and the poem.
*/
void SPI_Write(poem* buff1)
{
    uint16_t i;
    CSLow;

    EUSCI_B0->TXBUF = 0b00000010;
    while(!SPIFlag);
    SPIFlag=0;


    //Transmitting the address on flash chip.
    //EUSCI_B0->TXBUF = (buff1->address & 0x0000)>>8;
    EUSCI_B0->TXBUF = (buff1->startAddress & 0xFF00)>>8;
    while(!SPIFlag);
    SPIFlag=0;
    EUSCI_B0->TXBUF = (buff1->startAddress & 0xFF);
    while(!SPIFlag);
    SPIFlag=0;
    //Transmitting actual data to the specific address.
  //  memset(buff1->data,0,1000);
    for(i=0; i<buff1->size; i++)
    {
        EUSCI_B0->TXBUF = buff1->data[i];
        while(!SPIFlag);
        SPIFlag=0;
    }
    CSHigh;
}

/*
SPI_WriteIndex writes index.
*/
void SPI_WriteIndex(index* buff1)
{
    uint16_t i;
    uint16_t titleSize;

    CSLow;
    titleSize=sizeof(buff1->title);

    EUSCI_B0->TXBUF = 0b00000010;
    while(!SPIFlag);
    SPIFlag=0;

    //Transmitting the address on flash chip.
    //EUSCI_B0->TXBUF = (buff1->address & 0x0000)>>8;
    EUSCI_B0->TXBUF = (buff1->address & 0xFF00)>>8;
    while(!SPIFlag);
    SPIFlag=0;
    EUSCI_B0->TXBUF = (buff1->address & 0xFF);
    while(!SPIFlag);
    SPIFlag=0;
    //Transmitting actual data to the specific address.
  //  memset(buff1->data,0,1000);
    for(i=0; i<titleSize; i++)
    {
        EUSCI_B0->TXBUF = buff1->title[i];
        while(!SPIFlag);
        SPIFlag=0;
    }
    CSHigh;
}
/*
SPI_Read reads from flash.
*/
void SPI_Read(readPoem* poem1 )
{
     uint16_t i;
    char ReadBuff[1000]={1};



    EUSCI_B0->TXBUF = (poem1->startAddress & 0xFF00)>>8;
    while(!SPIFlag);
    SPIFlag=0;
    EUSCI_B0->TXBUF =(poem1->startAddress & 0xFF);
    while(!SPIFlag);
    SPIFlag=0;

    for(i=0; i<poem1->size; i++)
    {
        TXData=0b00000000;
        EUSCI_B0->TXBUF = TXData;
        while(!SPIFlag);
        SPIFlag=0;
        poem1->data[i] = EUSCI_B0->RXBUF;

    }
    CSHigh;

}
/*
SPI_ReadCommand sends the read command to the device.
*/
void SPI_ReadCommand(void)
{
    CSLow;
    TXData=0b00000011;
    EUSCI_B0->TXBUF = TXData;
    while(!SPIFlag);
    SPIFlag=0;

}




