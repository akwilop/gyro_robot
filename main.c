#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "leds.h"

int offset_gyr = 0, flag = 0;

signed int main(void) {
	
	motorDriverInit();
	ledsInitialize();
	btInitialize();
	sysTickInitialize(15);
	gyro_init();
	
	welcome();
	waitms(1000);
	offset_gyr	= iGyrRead_avg(0xFFFF, 'z');
	flag = 1;
	
	while(1) {
	
		
		
	}
}
