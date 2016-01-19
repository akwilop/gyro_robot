#include "sysTick.h"

#define SENSITIVITY 15

void sysTickInitialize(int ms) {	
	if(ms > 345) ms = 345;
	SysTick_Config(SystemCoreClock/1000*ms);	
}

extern int offset_gyr, flag;
signed short integral = 0;
unsigned int sample= 0, prev_sample = 0;

void SysTick_Handler() {
	
	if(flag) {
		prev_sample = sample;
		sample = ((iGyrReadZ() - offset_gyr) - ((iGyrReadZ() - offset_gyr) % SENSITIVITY)) / SENSITIVITY;
		integral += (sample + prev_sample) / 2;
		sendShort(integral);
	}
	
}
