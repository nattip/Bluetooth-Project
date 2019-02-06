#ifndef __TIMERS_H
#define __TIMERS_H

#define STOP_WDT WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD //define stop wdt


void timerA_Init(); //initialize TimerA
void TA1_0_IRQHandler ();   //TimerA IRQ Handler

#endif
