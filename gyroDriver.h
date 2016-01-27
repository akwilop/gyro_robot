#ifndef GYRO_DRIVER_H
#define GYRO_DRIVER_H

#include "gyro.h"
#include "bt.h"
#include "rgb.h"
#include "motorDriver.h"

void offset_gyro(void);
void calculate_hdg(void);
int get_hdg(void);
signed short get_course(void);
void turn(int);
void setTracks(int, int);
void waitms(int);

#endif
