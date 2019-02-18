#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include <string.h>
#include <SPI.h>
#include <Flash.h>

/**
 * main.c
 */

extern int RXData;
extern int TXData;

char datahelp[1000]={0};
uint16_t plz;
char wut='9';
char BLEwut='9';
uint8_t UARTflag;
uint8_t BLEflag;

void main(void)
{

    uint16_t i;
    uint16_t j = 0;

    SPI_Init();
    UART0_Init();
    UART2_Init();
    __enable_irq();
    char dataString[1000] = { 0 };
    uint8_t StoreFlag = 0;
    uint8_t ReadFlag = 0;
    uint8_t IndexFlag = 0;
    uint8_t MemoryFlag = 0;
    uint8_t EraseFlag = 0;
    uint8_t ClearFlag = 0;
    uint8_t ClearCheck = 0;
    uint16_t MemorySlot = 0;

    poem poem1;
    poem1.startAddress = 0x0000 + 500;

    readPoem test1;
    test1.startAddress = 0x0000;
    test1.size = 100;

    index Storage1;
    Storage1.address = 00;
    uint8_t strlength = 0;
    char addressbuff[4] = { 0 };
    char sizeBuff[4] = { 0 };\
    char* pointpoint1;
    char* pointpoint2;
    char* strPoint;
    char* strPoint2;
    char* UARTptr;
    char* UARTSlotptr;
    char stringBuff[20] = { 0 };
    char indexReading[12] = { 0 };
    char size[20] = { 0 };
    char addressStore[5] = { 0 };
    char sizeStore[4] = { 0 };
    uint16_t MemoryLeft = { 0 };
    uint8_t boardRestart = 1;
    uint16_t addressInt;
    uint16_t sizeInt;
    uint8_t EraseIndex = 0;
    char trigger[3] = "//";


    char bs;
    char bstx='p';
    char UARTcontent[1000]={0};
    char UARTstoreSize[4]={0};
    char memoryUART[4]={0};
    uint16_t UARTsize;

    char* maybe;
    uint16_t numbertest;
    while (1)
    {

        //The either inputs from the USB UART line, or the BLE line
        //Depending on the users input different flags are triggered

        //Writing command
        if(wut=='0')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            StoreFlag = 1;
            memset(UARTcontent,0,1000);
            for(i=0;i<3;i++)
            {
           // UARTstoreSize[i]=UART0_Read();
            UARTstoreSize[i]=UART0_Read();
            UARTsize=strtol(UARTstoreSize,&UARTptr,10);
            }
            for(i=0;i<UARTsize;i++)
            {
               // UARTcontent[i]=UART0_Read();
                UARTcontent[i]=UART0_Read();
            }
            wut='9';
            BLEwut='9';
        }
        //Read command
        else if(wut=='1' || BLEwut=='1')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            ReadFlag = 1;
            char t;
            if(UARTflag==1)
            {
            t=UART0_Read();
            }
            else if(BLEflag==1)
            {
            t=UART2_Read();
            }
            MemorySlot=t-'0';
            wut='9';
            BLEwut='9';
        }
        //Index command
        else if(wut=='2' || BLEwut =='2')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            IndexFlag = 1;
            wut='9';
            BLEwut='9';

        }
        //Memory command
        else if(wut=='5' || BLEwut =='5')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            MemoryFlag = 1;
           // MemorySlot=UART0_Read();
            wut='9';
            BLEwut='9';
        }
        //Erase command
        else if(wut=='3'|| BLEwut == '3')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            EraseFlag = 1;
            char h;
            if(UARTflag==1)
            {
            h=UART0_Read();
            }
            else if(BLEflag==1)
            {
            h=UART2_Read();
            }
            EraseIndex=h-'0';
            wut='9';
            BLEwut='9';
        }
        //Clear command
        else if(wut=='4' || BLEwut== '4')
        {
            EUSCI_A0->IE &=~ 0x01; //Interrupt disable
            EUSCI_A2->IE &=~ 0x01;
            ClearFlag = 1;
            wut='9';
            BLEwut='9';
        }

        //Start of storing poem onto flash
        if (StoreFlag == 1)
        {
            if (ClearCheck == 1)
            {
                poem1.startAddress = 500;
                ClearCheck = 0;
               // wut='9';
            }
            // char dataString[] = "Test 1\n\n";
            memset(poem1.data,0,1000);
            strcpy(poem1.data, UARTcontent);
            poem1.size = strlen(poem1.data);
            FlashTitle(&poem1);

            //From slot 1-10
            Storage1.slot = FlashCheckIndex(); //Checks for the first available slot for Index value.
            if (Storage1.slot < 11 && Storage1.slot != 0)
            {
                test1.startAddress = (Storage1.slot - 1) * 50;
                test1.size = 50;
                SPI_ReadCommand();
                SPI_Read(&test1);
                strPoint = strstr(test1.data, trigger);
                strcpy(stringBuff, strPoint);
                //printf("%s", stringBuff);
                for (i = 2; i < 13; i++)
                {
                    indexReading[j] = stringBuff[i];
                    j++;
                }
                j = 0;
                strPoint2 = strstr(indexReading, trigger);
                strcpy(size, strPoint2);
                // strlength=strlen(size);
                memset(addressStore, 0, 5);
                strncpy(addressStore, indexReading, strlen(size));
                size[0] = size[2];
                size[1] = size[3];
                size[2] = size[4];
                size[3] = size[5];
                strncpy(sizeStore, size, 4);
                addressInt = strtol(addressStore, &pointpoint1, 10);
                sizeInt = strtol(sizeStore, &pointpoint2, 10);
                if (boardRestart == 1)
                {
                    poem1.startAddress = addressInt + sizeInt;
                    boardRestart = 0;
                }

                else
                {
                    poem1.startAddress = addressInt + sizeInt + 1;
                }
                EUSCI_A0->IE |= 0x01;
            }
            //First slot of memory
            if (Storage1.slot == 0)
            {
                poem1.startAddress = 500;
            }

            //Parsing up the title, address and size.
            if (Storage1.slot < 11)
            {
                strcpy(Storage1.title, poem1.title);
                strcat(Storage1.title, "//");
                sprintf(addressbuff, "%d", poem1.startAddress);
                strcat(Storage1.title, addressbuff);
                strcat(Storage1.title, "//");
                sprintf(sizeBuff, "%d", poem1.size);
                strcat(Storage1.title, sizeBuff);

                Storage1.address = Storage1.slot * 50;
                //Writing index
                SPI_WriteLatch();
                SPI_WriteIndex(&Storage1);

                if (Storage1.slot == 0)
                {
                    test1.startAddress = 0;
                }
                else
                {
                    test1.startAddress = (Storage1.slot) * 50;
                }

                test1.size = 50;
                SPI_ReadCommand();
                SPI_Read(&test1);
                strPoint = strstr(test1.data, trigger);
                strcpy(stringBuff, strPoint);
                //printf("%s", stringBuff);
                for (i = 2; i < 13; i++)
                {
                    indexReading[j] = stringBuff[i];
                    j++;
                }
                j = 0;
                strPoint2 = strstr(indexReading, trigger);
                strcpy(size, strPoint2);
                // strlength=strlen(size);
                memset(addressStore, 0, 5);
                strncpy(addressStore, indexReading, strlen(size));
                size[0] = size[2];
                size[1] = size[3];
                size[2] = size[4];
                size[3] = size[5];
                strncpy(sizeStore, size, 4);
                addressInt = strtol(addressStore, &pointpoint1, 10);
                sizeInt = strtol(sizeStore, &pointpoint2, 10);
                //Writing poem to starting flash
                SPI_WriteLatch();
                SPI_Write(&poem1);
                poem1.startAddress = addressInt + sizeInt + 1;

            }
            else
            {
                ;              //no slot available.
            }

            StoreFlag = 0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }
        //Reading command
        else if (ReadFlag == 1)
        {
            memset(test1.data,0,1000);
            //Read from flash
            FlashRead(&test1, MemorySlot);
            if(UARTflag==1)
            {
            UART0_Write(&test1);
            while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A0->TXBUF='\r';
            }
            else if(BLEflag==1)
            {
            UART2_Write(&test1);
            while(!(EUSCI_A2->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A2->TXBUF='\r';
            }
            UARTflag=0;
            BLEflag=0;
            ReadFlag = 0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }
        //Index command
        else if (IndexFlag == 1)
        {
            if(UARTflag==1)
            {
            for (i = 0; i < 10; i++)
            {
                test1.startAddress = i * 50;
                test1.size = 50;
                //read index from flash
                SPI_ReadCommand();
                SPI_Read(&test1);
                UART0_WriteIndex(&test1);
            }
            while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A0->TXBUF='\r';
            }
            if(BLEflag==1)
            {
            for (i = 0; i < 10; i++)
            {
                test1.startAddress = i * 50;
                test1.size = 50;
                //read index from flash
                SPI_ReadCommand();
                SPI_Read(&test1);
                UART2_WriteIndex(&test1);
            }
            while(!(EUSCI_A2->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A2->TXBUF='\r';
            }

            IndexFlag = 0;
            UARTflag=0;
            BLEflag=0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }
        //Memory command
        else if (MemoryFlag == 1)
        {
            //Pulling last address stored for a poem
            MemoryLeft = FlashMemory(&test1);
            //Converting address+size to string via sprintf
            sprintf(memoryUART,"%d",MemoryLeft);
            for(i=0;i<4;i++)
            {
            if(UARTflag==1)
            {
            while(!(EUSCI_A0->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A0->TXBUF=memoryUART[i];   //write into the buffer
            }
            else if(BLEflag==1)
            {
            while(!(EUSCI_A2->IFG & 0x02)) {}   //wait until UART send suffer is full
            EUSCI_A2->TXBUF=memoryUART[i];   //write into the buffer
            }
            }
            MemoryFlag = 0;
            UARTflag=0;
            BLEflag=0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }
        //Erase command
        else if (EraseFlag == 1)
        {
            //Erase based off of the slot indicated by user
            FlashErase(&test1, EraseIndex);
            EraseFlag = 0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }
        //Clear command
        else if (ClearFlag == 1)
        {
            //  Clear all of flash
            char dataClear[1000] = { 0 };
            // memset(dataString, 0, 1000);
            poem1.size = sizeof(dataClear);
            strncpy(poem1.data, dataClear, 1000);
            for (i = 0; i < 10; i++)
            {
                poem1.startAddress = i * 1000;
                SPI_WriteLatch();
                SPI_Write(&poem1);
            }
            ClearCheck = 1;
            ClearFlag = 0;
            EUSCI_A0->IE |= 0x01;
            EUSCI_A2->IE |= 0x01;
        }

    }

}

void EUSCIA0_IRQHandler(void)
{

    wut= EUSCI_A0->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    UARTflag=1;
}

void EUSCIA2_IRQHandler(void)
{

    BLEwut= EUSCI_A2->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    BLEflag=1;
}

