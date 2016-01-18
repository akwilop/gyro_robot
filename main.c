#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "leds.h"

int main(void) {
	
	motorDriverInit();
	ledsInitialize();
	btInitialize();
	sysTickInitialize(100);
	gyro_init();
	
	welcome();
	
	while(1) {
	
		
		
	}
}
