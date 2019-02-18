/*
 * Flash.c
 *
 *  Created on: Feb 9, 2019
 *      Author: andre
 */

#include "msp.h"    //include necessary libraries
#include <stdio.h>
#include <timers.h>
#include <IO.h>
#include <UART.h>
#include <string.h>
#include <SPI.h>
#include <Flash.h>

//        int FlashStore(poem* store)
//        {
//            ;
//        }

index Slot[10];
uint8_t i;
poem EraseWrite;
readPoem EraseRead;
index EraseIndex;

char indexClear[50] = { 0 };
char poemClear[1000] = { 0 };
char remander[1000];
char * start;
char * end;
uint16_t titleLength;
char test[20] = { 0 };
char *titleTest;
char titleTrigger[5] = "\n\n";

char size[10] = { 0 };
char addressStore[5] = { 0 };
char sizeStore[4] = { 0 };
char indexReading[12] = { 0 };
char stringBuff[20] = { 0 };
uint16_t addressInt = 0;
uint16_t sizeInt = 0;
char addressbuff[4] = { 0 };
char sizeBuff[4] = { 0 };
//char* strPoint;
//char* strPoint2;
char* pointpoint1;
char* pointpoint2;
uint8_t j;
uint8_t k;

/*
FlashRead sets up for flash to read the correct
*/
int FlashRead(readPoem* read, uint8_t readSlot)
{
    char* str;
    char* str2;
    char trigger[3] = "//";

    if (readSlot < 10)
    {
        read->startAddress = readSlot * 50;
        read->size = 50;
        SPI_ReadCommand();
        SPI_Read(read);

        str = strstr(read->data, trigger);
        strcpy(stringBuff, str);
        for (i = 2; i < 13; i++)
        {
            indexReading[j] = stringBuff[i];
            j++;
        }
        j = 0;
        str2 = strstr(indexReading, trigger);
        strcpy(size, str2);
        memset(addressStore, 0, 5);
        strncpy(addressStore, indexReading, strlen(size));
        size[0] = size[2];
        size[1] = size[3];
        size[2] = size[4];
        size[3] = size[5];
        strncpy(sizeStore, size, 4);
        addressInt = strtol(addressStore, &pointpoint1, 10);
        sizeInt = strtol(sizeStore, &pointpoint2, 10);
        str = strstr(read->data, trigger);

        read->startAddress = addressInt;
        read->size = sizeInt;
        SPI_ReadCommand();
        SPI_Read(read);

    }
}

int FlashIndex(poem* index)
{
    ;
}
/*
FlashMemory calculates the amount of memory left.
*/
uint16_t FlashMemory(readPoem* memory)
{
    uint8_t LastSlot = 0;
    char* str;
    char* str2;
    char helptest[50] = { 0 };
    char trigger[3] = "//";
    uint16_t Extra=0;
    LastSlot = FlashCheckIndex();

    memory->startAddress = 50 * (LastSlot - 1);
    memory->size = 50;
    SPI_ReadCommand();
    SPI_Read(memory);

    strcpy(helptest, memory->data);
    str = strstr(helptest, trigger);
    strcpy(stringBuff, str);
    printf("%s", stringBuff);
    for (k = 2; k < 13; k++)
    {
        indexReading[j] = stringBuff[k];
        j++;
    }
    j = 0;
    str2 = strstr(indexReading, trigger);
    memset(size, 0, 10);
    strcpy(size, str2);
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

    Extra=8191-(addressInt+sizeInt)-500;
    return Extra;

}

/*
FlashErase erases a slot of memory.
*/

int FlashErase(readPoem* erase, uint8_t indexSlot)
{
    char* str;
    char* str2;
    char helptest[1000] = { 0 };
    char trigger[3] = "//";
    uint8_t help;
    help = FlashCheckIndex();

    erase->size = 50;
    for (i = 0; i < help; i++)
    {
        erase->startAddress = i * 50;
        SPI_ReadCommand();
        SPI_Read(erase);
        if (erase->data[0] != 0)
        {
            //Slot[i].title;
            strcpy(helptest, erase->data);
            //str = strstr(erase->data, trigger);
            str = strstr(helptest, trigger);
            strcpy(stringBuff, str);
            printf("%s", stringBuff);
            for (k = 2; k < 13; k++)
            {
                indexReading[j] = stringBuff[k];
                j++;
            }
            j = 0;
            str2 = strstr(indexReading, trigger);
            memset(size, 0, 10);
            strcpy(size, str2);
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

            Slot[i].poemAddress = addressInt;
            Slot[i].size = sizeInt;

            start = (erase->data);
            end = (strstr(start, trigger));
            titleLength = (end) - start + 1;
            memset(Slot[i].title, 0, sizeof(Slot[i].title));
            strncpy(Slot[i].title, start, titleLength + 1);

        }
    }
    for (i = indexSlot; i < help; i++)
    {
        if ((i + 1) == help)
        {
            break;
        }
        EraseRead.startAddress = Slot[i + 1].poemAddress;
        EraseRead.size = Slot[i + 1].size;

        SPI_ReadCommand();
        SPI_Read(&EraseRead);
        strcpy(EraseWrite.data, EraseRead.data);
        EraseWrite.startAddress = Slot[i].poemAddress;
        EraseWrite.size = Slot[i + 1].size;
        SPI_WriteLatch();
        SPI_Write(&EraseWrite);

        // strcat(Slot[i + 1].title, "//");
        sprintf(addressbuff, "%d", Slot[i].poemAddress);
        strcat(Slot[i + 1].title, addressbuff);
        strcat(Slot[i + 1].title, "//");
        sprintf(sizeBuff, "%d", Slot[i + 1].size);
        strcat(Slot[i + 1].title, sizeBuff);

        Slot[i + 1].address = i * 50;

        SPI_WriteLatch();
        SPI_WriteIndex(&Slot[i + 1]);
        Slot[i + 1].poemAddress = Slot[i].poemAddress + Slot[i + 1].size;
    }
    memset(Slot[help - 1].title, 0, 50);

    SPI_WriteLatch();
    Slot[help - 1].address = (help - 1) * 50;
    SPI_WriteIndex(&Slot[help - 1]);

    memset(EraseWrite.data, 0, 1000);
    EraseWrite.size = 1000;
    EraseWrite.startAddress = Slot[help - 1].poemAddress;
    SPI_WriteLatch();
    SPI_Write(&EraseWrite);
}

int FlashClear(poem* clear)
{
    ;
}

/*
FlashTitle finds the title from the poem.
*/
int FlashTitle(poem* title)
{
//    char titleTrigger[5] = "\n\n";
//    char * start;
//    char * end;
//    uint16_t titleLength;

    start = (title->data);
    end = (strstr(start, titleTrigger));
    titleLength = (end) - start + 1;
    memset(title->title, 0, sizeof(title->title));
    strncpy(title->title, start, titleLength);
    //  strpy(title->title, title->data, titleLength);

}


/*
FlashCheckIndex checks for the next open slot of memory.
*/
int FlashCheckIndex(void)
{
    uint8_t i;
    readPoem IndexAvail;
    IndexAvail.size = 50;
    IndexAvail.startAddress = 0x0000;
    memset(IndexAvail.data, 0, sizeof(IndexAvail.data));
    // memset(IndexAvail.title,0,sizeof(IndexAvail.title));
    for (i = 0; i < 10; i++)
    {
        IndexAvail.startAddress = 0x0000 + i * 50;
        SPI_ReadCommand();
        SPI_Read(&IndexAvail);
        if ((IndexAvail.data[0] == 0))
        {
            return i;
        }
        else
        {
            IndexAvail.startAddress = (IndexAvail.startAddress) + 50;
        }

    }
}

int FlashAddressSize(readPoem* Help)
{
    // Help->data=
}
