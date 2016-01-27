#include "sysTick.h"

void sysTickInitialize(int ms) {	
	if(ms > 345) ms = 345;
	SysTick_Config(SystemCoreClock/1000*ms);	
}

void SysTick_Handler() {
	
	calculate_hdg();
	//send(get_hdg(), 3);
	
}
