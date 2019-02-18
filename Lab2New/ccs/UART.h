#ifndef __UART_H
#define __UART_H

void UART2_Init();
void UART0_Init();  //initialize UART0
char UART0_Read();  //Read info from UART0
void UART0_Write();
void UART0_WriteIndex();

#endif
