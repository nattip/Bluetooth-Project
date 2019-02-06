/*
 * SPI.h
 *
 *  Created on: Feb 4, 2019
 *      Author: andre
 */

#ifndef SPI_H_
#define SPI_H_


typedef struct WriteInfo
{
    char address[16];
    char data [1000];
}WriteBuffer;


void SPI_Init();
void EUSCIB0_IRQHandler();
void SPI_WriteLatch();
void SPI_WriteUnlatch();
void SPI_Write();
void SPI_Read();


#endif /* SPI_H_ */
