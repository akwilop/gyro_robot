#include "sysTick.h"

#define SENSITIVITY 40
#define SPD 30

void sysTickInitialize(int ms) {	
	if(ms > 345) ms = 345;
	SysTick_Config(SystemCoreClock/1000*ms);	
}

extern int offset_gyr, flag, drive;
signed short integral = 0, prev_integral = 0;
unsigned int sample= 0, prev_sample = 0,  corr = 1;

void SysTick_Handler() {
	
	if(flag) {
		prev_sample = sample;
		sample = ((iGyrReadZ() - offset_gyr) - ((iGyrReadZ() - offset_gyr) % SENSITIVITY)) / SENSITIVITY;
		sample = sample << 1;
		prev_integral = integral;
		integral += (sample + prev_sample) / 2;
		sendShort(integral);
		
		if(drive) {
				
			if(integral > 0) {
				setTracksSpeed(SPD, SPD - corr);
				if(integral + 50 > prev_integral) corr++;
			}
			else {
				setTracksSpeed(SPD - corr, SPD);
				if(integral - 50 < prev_integral) corr++;
			}
			if((integral * prev_integral) < 0) corr = 0;
					
		}
		else setTracksSpeed(0, 0);
		
	}
}
