/****************************************
 * Written by Natalie Tipton and Andrew Deradoorian
 * EGR 436 (102)
 * Dr. Brakora
 * Lab 1
 * 1/7/19
 * This code receives data from UART communication
 *      from the MSP432 to display the current tempo.
 *      It then takes user input through a single key stroke
 *      to increase or decrease the bpm by 2 or reset
 *      the tempo to 60. It then sends info through UART
 *      back to the MSP432 to make the adjustments.
 ****************************************/

#include <stdio.h>  //include necessary libraries
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <inttypes.h>
#include "SerialCom.h"


int main()
{
    unsigned char input;    //initialize variables and arrays
    int n = 1;
    unsigned char readBuff[1]={0};
    unsigned char writeBuff[1]={0};
    DWORD dwBytesRead = 0;
    DWORD dwBytesWrite = 0;


    HANDLE hSerial = handle_Init();     //set up handle tag to set up COM port
    dcb_Params(hSerial);                    //setup DCB parameters to set state of COM port

   //print instructions to user
    printf("Please press:\n*u* to increase tempo,\n*d* to decrease tempo,\n*r* to reset tempo to 60 bpm.\n*x to exit system.\n");

    while(1){       //infinite while loop
    input = getch();    //get character from user input without needing to press enter

    //if user entered u, d, or r
    if((input == 'u' && readBuff[0] <= 252)| (input == 'd' && readBuff[0] >=6) | input == 'r'){
        printf("\n\nInput: %c",input);  //print the user's intput
        writeBuff[0] =  input;  //saves input into write buffer

        if(!WriteFile(hSerial, writeBuff, n, &dwBytesWrite, NULL))  //write to user input to UART
            printf("Error writing UART\n");     //tell user if writing did not work

        if(!ReadFile(hSerial, readBuff, n, &dwBytesRead, NULL))   //read from UART
            printf("Error reading UART\n");     //tell user if reading did not work

        else printf("\nCurrent Tempo: %d",readBuff[0]);}        //if reading did work, write current tempo to user

    else if(input == 'x')   //if user entered x
        break;  //leave while loop
    }

    CloseHandle(hSerial);   //close COM port to the user

    return 0;
}
