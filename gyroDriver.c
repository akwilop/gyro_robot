#include "gyroDriver.h"

#define SENSITIVITY 50
#define SPEED 100
#define FIFO 10
#define Kp 4
#define Ki 0.01
#define Kd 0

signed short read = 0, prev_read = 0, actual_offset = 0, course = 0;
int integral, gyro_ready = 0;

void offset_gyro() {
	waitms(100);
	actual_offset = iGyrRead_avg(0xFF, 'z');
	waitms(100);
	integral = 0;
}

int gyro_init_flag = 1;

void calculate_hdg() {
	
	if(gyro_init_flag) {
		RGB(65, 0, 0);
		offset_gyro();
		gyro_init_flag = 0;
		RGB(0, 15, 0);
	}
	else {
		prev_read = read;
		read = ((iGyrReadZ() - actual_offset) - ((iGyrReadZ() - actual_offset) % SENSITIVITY)) / SENSITIVITY;
		read = read << 1;
		integral -= (prev_read + read) >> 1;
		gyro_ready = 1;
	}
}

int get_hdg() {
	return integral / 453;
}

signed short get_course() {
	return course;
}

void turn(int deg) {
	int prev_prop = 0, PID = 0;
	int prop = 0, integ = 0;
	int result = 0;
	offset_gyro();

	while(1) {
		if(gyro_ready) {
			prev_prop = prop;											
			prop = deg - get_hdg();		
			if((prop > -3) && (prop < 3))integ += (prop + prev_prop) / 2;		
			else integ = 0;
			PID = ((Kp * prop) + (Ki * integ));		//PI
			if(PID > 0) PID += 20;
			else if (PID < 0) PID -= 20;		//popr na gasienice
			if(PID > 50) PID = 50;					
			else if(PID < -50) PID = -50;		//ogr predkosci
			setTracks(PID, -PID);
			gyro_ready = 0;
			if(get_hdg() == deg) result ++;
			else result = 0;
			if(result == 10) {
				waitms(1);
				setTracks(0, 0);
				break;				
			}
		}
	}	
}

void setTracks(int left, int right) {
	if(left >= 0) {
		setLeftTrackDir(FORWARD);
		setLeftTrackSpeed(left);
	}
	else {
		setLeftTrackDir(REVERSE);
		setLeftTrackSpeed(-left);
	}
	if(right >= 0) {
		setRightTrackDir(FORWARD);
		setRightTrackSpeed(right);
	}
	else {
		setRightTrackDir(REVERSE);
		setRightTrackSpeed(-right);
	}
}

void waitms(int ms) {
	int inside, outside;	
	for(outside = 0; outside < ms; outside ++) {		
		for(inside = 0; inside < 8000; inside++);			
	}	
}

//void turn(int deg) {
//	int prev_prop = 0, PID = 0;
//	int prop = 0, integ = 0, deriv = 0;
//	int result = 0;
//	offset_gyro();
//	
//	while(1) {
//		if(gyro_ready) {
//			prev_prop = prop;
//			prop = deg - get_hdg();
//			integ += (prop + prev_prop) / 2;
//			if(Ki * integ > SPEED) integ = SPEED;
//			else if(Ki * integ < -SPEED) integ = -SPEED;
//			deriv = prop - prev_prop;
//			PID = ((Kp * prop) + (Ki * integ) + (Kd * deriv));
//			if(PID > SPEED) PID = SPEED;
//			else if(PID < -SPEED) PID = -SPEED;
//			setTracks(PID, -PID);
//			gyro_ready = 0;
//			if(get_hdg() == deg) result ++;
//			else result = 0;
//			if(result == 10) {
//				waitms(1);
//				setTracks(0, 0);
//				break;				
//			}
//		}
//	}	
//}
