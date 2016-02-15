#ifndef GYRO_DRIVER_H
#define GYRO_DRIVER_H

#include "gyro.h"
#include "bt.h"
#include "rgb.h"
#include "motorDriver.h"

#define SENSITIVITY 80	 /** Kwantyzacja danych z zyroskopu - przeciwdzialanie szumom/zakloceniom. Zmiana spowoduje bledne dzialanie funkcji gyroGetHdg oraz gyroTurn */																						 
#define Kp_turn			0.6	 /** Wspolczynnik czlonu proporcjonalnego regulatora PI regulujacego obrotu */	
#define Ki_turn			0.01 /** Wspolczynnik czlonu calkujacego regulatora PI regulujacego obrotu */
#define Kp_straight	0.6	 /** Wspolczynnik czlonu proporcjonalnego regulatora PI regulujacego jazde na wprost */
#define Ki_straight	0.01 /** Wspolczynnik czlonu calkujacego regulatora PI regulujacego jazde na wprost */

void gyroOffset(void);
void gyroCalculateHdg(void);
signed short gyroGetHdg(void);
void gyroTurn(signed short);
void gyroStraight(signed short, signed short);
void normal(signed short, signed short);
void setTracks(signed short, signed short);
void waitms(unsigned short);

#endif
