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

    uint8_t i;
    uint8_t previousData;
    char buffer[1000]={0};

    SPI_Init();

    char dataString []="DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

    WriteBuffer FlashWrite;
    FlashWrite.address=0000000000000000;
    strcpy(FlashWrite.data, dataString);
    FlashWrite.size=sizeof(dataString);

    poem poem1;
    poem1.startAddress=0x0000;
    poem1.size=99;
    poem1.title;

int pick=0;
    while(1)
       {


      if (pick==0)
      {
      SPI_WriteLatch();
      SPI_Write(&FlashWrite);
 //     SPI_WriteUnlatch();
      }

      else if (pick==1)
      {

          SPI_ReadCommand();
          SPI_Read(&poem1);

      }


      //  CSHigh;



       }
}


