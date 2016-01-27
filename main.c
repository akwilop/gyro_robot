#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"
#include "sysTick.h"
#include "i2c1.h"
#include "gyro.h"
#include "rgb.h"
#include "gyroDriver.h"

int test = 0;

int main(void) {
	
	rgbInitialize();
	motorDriverInit();
	btInitialize();
	gyro_init();
	sysTickInitialize(10);
	
	while(1) {
	
		waitms(300);
		RGB(0, 0, 15)
		turn(-360);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(360);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(-180);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(180);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(-90);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(90);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(-30);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		waitms(300);
		RGB(0, 0, 15)
		turn(30);	
		RGB(0, 15, 0);
		test++;
		send(test, 3);
		
	}
}
