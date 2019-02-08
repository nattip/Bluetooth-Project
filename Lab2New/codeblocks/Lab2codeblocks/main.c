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
    char input;    //initialize variables and arrays
    int n = 228;
    unsigned char readBuff[229]={0};
    char writeBuff[351]={0};
   //unsigned char writeBuff[1]={0};
    FILE *fp;
    char str[MAXCHAR];
    char* filename="D:\\Testpoem2.txt";
    const char* eot="00000100";
    fp=fopen(filename, "r");

    if (fp==NULL){
        printf("Could not open file %s",filename);
    }

    memset(writeBuff, 0, 351);
    while(fgets(str,MAXCHAR,fp)!=NULL)
    {
        printf("%s", str);
        strcat(writeBuff,str);

    }
    printf("\n\n");
    fclose(fp);
    strcat(writeBuff,eot);

    DWORD dwBytesRead = 0;
    DWORD dwBytesWrite = 0;


    HANDLE hSerial = handle_Init();     //set up handle tag to set up COM port
    dcb_Params(hSerial);                    //setup DCB parameters to set state of COM port

   //print instructions to user

    //input = str;    //get character from user input without needing to press enter
 //   for(int i=0; i<350;i++){
   // writeBuff[i] =  str[i];  //saves input into write buffer
//}
while(1)
{

if(getch()=='1'){

    if(!WriteFile(hSerial, writeBuff, n, &dwBytesWrite, NULL))  //write to user input to UART
    printf("Error writing UART\n");
}

else if(getch()=='2')
{
    if(!ReadFile(hSerial, readBuff, n, &dwBytesRead, NULL))   //read from UART
    printf("Error reading UART\n");     //tell user if reading did not work

 for (uint16_t i=0; i<229; i++)
    {
         printf("%c",readBuff[i]);
    }
memset(readBuff, 0, 229);
}
       //if reading did work, write current tempo to user

}

   // while(1){       //infinite while loop
     //tell user if writing did not work
/*
        if(!ReadFile(hSerial, readBuff, n, &dwBytesRead, NULL))   //read from UART
            printf("Error reading UART\n");     //tell user if reading did not work

        else printf("\nCurrent Tempo: %d",readBuff[0]);}        //if reading did work, write current tempo to user

    else if(input == 'x')   //if user entered x
        break;  //leave while loop

*/
  //  }

    CloseHandle(hSerial);   //close COM port to the user

    return 0;
}
