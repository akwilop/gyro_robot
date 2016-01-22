#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "rgb.h"

int offset_gyr = 0, flag = 0, drive = 0;

signed int main(void) {
	
	motorDriverInit();
	btInitialize();
	sysTickInitialize(5);
	gyro_init();
	rgbInitialize();
	
	RGB(50, 0, 0);
	
	waitms(1000);
	offset_gyr	= iGyrRead_avg(0x00FF, 'z');
	flag = 1;
	setTracksDir(FORWARD, FORWARD);
	
	while(1) {
	
		
		
	}
}
