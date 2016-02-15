/** \file sysTick.h
		\brief Handler przerwania co 2.5 ms*/

#include "sysTick.h"

extern uint8_t state;

void sysTickInitialize(int time) {	
	if(time > 345) time = 345;
	SysTick_Config(SystemCoreClock/10000*time);
	NVIC_SetPriority(SysTick_IRQn, 0);
}

void SysTick_Handler() {
	gyroCalculateHdg();
	if(state == READY) rgbStrobe();
	else if(state == BUSY) rgbRedBlue();
	else if(state == COMPLETE) RGB(0, 255, 0);
}
