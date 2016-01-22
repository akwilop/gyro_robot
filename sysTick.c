#include "sysTick.h"

#define SENSITIVITY 60
#define SPD 50

void sysTickInitialize(int ms) {	
	if(ms > 345) ms = 345;
	SysTick_Config(SystemCoreClock/1000*ms);	
}

extern int offset_gyr, flag, drive;
signed short integral = 0, prev_integral = 0;
unsigned int sample= 0, prev_sample = 0, spd = 0, fcorr = 0;
float scorr = 0;

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
				RGB(0, 0, 50);
				setTracksSpeed(spd, spd - scorr - fcorr);
				if(integral + 50 > prev_integral) scorr += 0.1;
				if(integral > 1500) fcorr++;
				else fcorr = 0;
			}
			else {
				RGB(50, 0, 0);
				setTracksSpeed(spd - scorr - fcorr, spd);
				if(integral - 50 < prev_integral) scorr += 0.1;
				if(integral < 1500) fcorr++;
				else fcorr = 0;
			}
			
			if((integral * prev_integral) < 0) scorr = 0;
			if(spd < SPD) spd++;		
			
		}
		else {
			setTracksSpeed(0, 0);
			RGB(0, 50, 0);
		}
		
	}
}
