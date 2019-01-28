#include "msp.h"
#include <stdio.h>
#include <timersinit.h>

/**
 * main.c
 */
void UART0_init(void);
uint16_t changedSpeed=60;
void main(void)
{
    __disable_irq();
    uint16_t TopValue=4096;
    uint16_t basespeed=60;
    uint16_t ratio;

    uint16_t AlteredCount;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P2->SEL1 &=~1;
	P2->SEL0 &=~1;
	P2->DIR |=1;

	NVIC_SetPriority(TA1_0_IRQn,3);
	NVIC_EnableIRQ(TA1_0_IRQn);

	UART0_init();
	timerA_Init();

	while(1)

{
	   ratio=(100*basespeed)/changedSpeed;
	   AlteredCount=(TopValue*ratio)/100;
	   TIMER_A1->CCR[0]=AlteredCount/2;

//    while((TIMER_A1->CCTL[0] & 1)==0);
  //      TIMER_A1->CCTL[0] &=~1;
    //    P2->OUT ^=2;

}
}

void TA1_0_IRQHandler (void)
{
    TIMER_A1->CCTL[0] &=~1;
    P2->OUT ^=1;

    while(!(EUSCI_A0->IFG & 0x02)) {}
    EUSCI_A0->TXBUF=changedSpeed;
    while(!(EUSCI_A0->IFG & 0x02)) {}
    EUSCI_A0->TXBUF='\n';
}

void UART0_init(void)
{
    EUSCI_A0->CTLW0 |=1;
    EUSCI_A0->MCTLW=0;
    EUSCI_A0->CTLW0=0x0081;
    EUSCI_A0->BRW=26;
    P1->SEL0 |=0x0C;
    P1->SEL1 &=~ 0x0C;
    EUSCI_A0->CTLW0 &=~ 1;
}
