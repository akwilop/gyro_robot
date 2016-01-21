#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "leds.h"

int offset_gyr = 0, flag = 0, drive = 0;

signed int main(void) {
	
	motorDriverInit();
	ledsInitialize();
	btInitialize();
	sysTickInitialize(11);
	gyro_init();
	
	welcome();
	waitms(100);
	offset_gyr	= iGyrRead_avg(0x00FF, 'z');
	flag = 1;
	setTracksDir(FORWARD, FORWARD);
	ledGreenOn();
	
	
	while(1) {
	
		
		
	}
}
