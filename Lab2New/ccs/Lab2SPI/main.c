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

    uint16_t i;
    uint8_t previousData;
    char buffer[1000]={0};

    SPI_Init();
    UART0_Init();
    char Uartstuff[229];
    char dataString []="qweqweqwe";

    WriteBuffer FlashWrite;
    FlashWrite.address=0000000000000001;
    strcpy(FlashWrite.data, Uartstuff);
    FlashWrite.size=sizeof(Uartstuff);

    poem poem1;
    poem1.startAddress=0x0000;
    poem1.size=228;
    poem1.title;

int pick=2;
    while(1)
       {
      if (pick==0)
      {
      SPI_WriteLatch();
      SPI_Write(&FlashWrite);
      pick=1;
 //     SPI_WriteUnlatch();
      }

      else if (pick==1)
      {

          SPI_ReadCommand();
          SPI_Read(&poem1);
          pick=3;
      }

      else if(pick==2)
      {

          for(i=0;i<229;i++)
          {
          Uartstuff[i]= UART0_Read();
          }
          memcpy(FlashWrite.data, Uartstuff,229);
          FlashWrite.size=sizeof(Uartstuff);
          pick=0;
      }
      else if(pick==3)
      {
       UART0_Write(&poem1);
 //     while(1);
      pick = 2;
      }


      //  CSHigh;



       }
}


