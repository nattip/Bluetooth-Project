/*
 * SerialCom.c
 *
 *  Created on: Jan 9, 2019
 *  Author: Andrew Deradoorian and Natalie Tipton
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <inttypes.h>
#include "SerialCom.h"

HANDLE handle_Init(void){
    HANDLE hSerial;                                                               //create handle variable
    hSerial = CreateFile("COM7",                                            //setup COM port 7
                                  GENERIC_READ | GENERIC_WRITE,    //read or write
                                  0,
                                  0,
                                  OPEN_EXISTING,                                //opening com port that exists
                                  FILE_ATTRIBUTE_NORMAL,                //normal file attribute
                                  0);

    if(hSerial == INVALID_HANDLE_VALUE){                        //if hSerial value returned is not okay
        if(GetLastError() == ERROR_FILE_NOT_FOUND){     //if COM port doesn't exist
            printf("File not found error.\n\n");                          //tell user file not found
        }
        else printf("Other error\n\n");                                  //tell user there was another error leading to invalid handle value
    }
    return hSerial;                                                             //return value of hSerial to main
}

void dcb_Params(HANDLE hSerial){
DCB dcbSerialParams = {0};                                                  //create DCB structure

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);     //set size parameter

    if(!GetCommState(hSerial, &dcbSerialParams)){               //fill in parameters currently in use by the serial port
        printf("Error getting state.\n\n");                                   //tell user if the above line could not get the state
    }

    dcbSerialParams.BaudRate = CBR_115200;                      //set baud rate
    dcbSerialParams.ByteSize = 8;                                         //set byte size
    dcbSerialParams.StopBits - ONESTOPBIT;                          //set stop bits
    dcbSerialParams.Parity = NOPARITY;                                //no parity

    if(!SetCommState(hSerial, &dcbSerialParams)){               //apply settings from above to serial port
        printf("Error setting serial port state.\n\n");                 //inform user of an error setting the serial port
    }
}

void timeouts_Init(HANDLE hSerial){
 COMMTIMEOUTS timeouts = {0};                                      //create commtimeouts struct

    timeouts.ReadIntervalTimeout = 50;                              //how long to wait between receiving characters before timing out
    timeouts.ReadTotalTimeoutConstant = 50;                    //how long to wait before returning from read
    timeouts.ReadTotalTimeoutMultiplier = 10;                    //how much additional time to wait before returning for each byte requested in read
    timeouts.WriteTotalTimeoutConstant = 50;                   //how long to wait before returning from write
    timeouts.WriteTotalTimeoutMultiplier = 10;                   //how much additional time to wait before returning for each byte requested in write

    if(!SetCommTimeouts(hSerial, &timeouts)){               //Apply settings from above to serial port
        printf("Error with comm timeouts.\n\n");                 //inform user of an error setting the serial port
    }
}
