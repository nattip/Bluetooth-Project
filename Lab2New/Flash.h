/*
 * Flash.h
 *
 *  Created on: Feb 9, 2019
 *      Author: andre
 */

#ifndef FLASH_H_
#define FLASH_H_



typedef struct
{
    uint16_t startAddress;
    uint16_t endAddress;
    char title[50];
    char data[1000];
    uint16_t size;

}poem;

typedef struct
{
    uint16_t startAddress;
    uint16_t  size;
    char data[1000];
}readPoem;

typedef struct
{
    uint16_t address;
    uint16_t poemAddress;
    char title[50];
    uint16_t size;
    uint8_t titleSize;
    uint8_t slot;
}index;

        int FlashStore(poem*);
        int FlashRead(readPoem*);
        int FlashIndex(poem*);
        int FlashMemory(poem*);
        int FlashErase(readPoem*,uint8_t);
        int FlashClear(poem*);
        int FlashTitle(poem*);
        int FlashCheckIndex();



#endif /* FLASH_H_ */
