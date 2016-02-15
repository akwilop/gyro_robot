#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "rgb.h"
#include "gyroDriver.h"

int main(void) {
	
	rgbInitialize();
	motorDriverInit();
	btInitialize();
	gyroInit();
	rgbNavLitesInitialize();
	sysTickInitialize(25);
	
	rgbSetBrightness(100);
	
	while(1) {	
			
	}
}
