#include "sysTick.h"
							
void sysTickInitialize(int ms) {	
	if(ms > 345) ms = 345;
	SysTick_Config(SystemCoreClock/1000*ms);	
}

signed long max = -444, min = -444; 
signed long diff;
signed short read;

void SysTick_Handler() {
	
	read = iGyrReadZ();
	if(read < min) min = read;
	if(read > max) max = read;
	diff = max - min;
	sendShort(iGyrReadZ());
	
}
