/*
 * SPI.h
 *
 *  Created on: Feb 4, 2019
 *      Author: andre
 */

#ifndef SPI_H_
#define SPI_H_

#define CSHigh          P2->OUT |= BIT5;                   //CS Bit
#define CSLow           P2->OUT &=~ BIT5;                     //CS Bit


typedef struct
{
    uint16_t address;
    char data [1000];
    uint16_t size;
}WriteBuffer;

typedef struct
{
    uint16_t startAddress;
    uint16_t endAddress;
    char title[1000];
    uint16_t size;

}poem;

void SPI_Init();
void EUSCIB0_IRQHandler();
void SPI_WriteLatch();
void SPI_WriteUnlatch();
void SPI_Write();
void SPI_ReadCommand();
void SPI_Read();


#endif /* SPI_H_ */
