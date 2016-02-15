/** \file gyroDriver.h
		\brief Funkcjonalnosci wykozystujace zyroskop*/
						
#include "gyroDriver.h"

signed short read = 0, prev_read = 0, actual_offset = 0;
int integral;
uint8_t gyro_ready = 0;

/** \fn void gyroOffset(void)
		\brief Funkcja usuwajaca offset z odczytow zyroskopu z osi Z oraz
					 zerujaca aktualny kurs(wartosc ktora zwroci 'gyroGetHdg()'*/

void gyroOffset() {
	waitms(200);
	actual_offset = gyroReadAvg(0xFF, 'z');
	waitms(200);
	integral = 0;
}

int gyro_init_flag = 1;

/** \fn void gyroCalculateHdg(void)
		\brief Funkcja obliczajaca kurs na podstawie danych predkosci obrotowej z zyroskopu.
		\brief Nalezy ja umiescic tak, aby byla wykonywana cyklicznie co 2.5ms.
					 Bez niej pozostale funkcje nie beda mogly dzialac prawidlowo*/

void gyroCalculateHdg() {
	
	if(gyro_init_flag) {
		waitms(200);
		gyroOffset();
		gyro_init_flag = 0;
	}
	else {
		prev_read = read;
		read = ((gyroReadZ() - actual_offset) - ((gyroReadZ() - actual_offset) % SENSITIVITY)) / SENSITIVITY;
		read = read << 1;
		integral -= (prev_read + read) >> 1;
		gyro_ready = 1;
	}
	
}

/** \fn signed short gyroGetHdg(void)
		\brief Funkcja zwracajaca kurs robota*/

signed short gyroGetHdg() {
	return integral / 112.2;
}

/** \fn void gyroTurn(signed short deg_10)
		\brief Funkcja sluzaca do obrotu robota o okreslony kat.
		\brief Prosze zwrocic uwage, ze funkcja wywoluje 'gyroOffset()' co zeruje aktualna wartosc zwracana przez 'gyroGetHdg()'.
		\param deg_10 Wybor kata do obrotu w dziesiatych czesciach stopnia
		\n '+' zgodnie ze wskazowkami zegara
		\n '-' przeciwnie do ruchu wskazowek zegara*/

void gyroTurn(signed short deg_10) {
	int PID = 0;
	int prop = 0, integ = 0;
	int result = 0;
	gyroOffset();

	while(1) {
		if(gyro_ready) {
			prop = deg_10 - gyroGetHdg();		
			if((prop > -30) && (prop < 30))integ += prop;		
			else integ = 0;		
			PID = ((Kp_turn * prop) + (Ki_turn * integ));
			if(PID > 45) PID = 45;					
			else if(PID < -45) PID = -45;
			setTracks(PID, -PID);
			gyro_ready = 0;
			if((prop < 4) && (prop > -4)) result ++;
			else result = 0;
			if(result == 10) {
				setTracks(0, 0);
				break;				
			}
		}
	}	
}

/** \fn void gyroStraight(signed short ticks, signed short speed)
		\brief Funkcja do jazdy robota na wprost.
		\brief Prosze zwrocic uwage, ze funkcja wywoluje 'gyroOffset()' co zeruje aktualna wartosc zwracana przez 'gyroGetHdg()'.
		\param ticks Czas jazdy * 2.5ms
		\param speed Szybkosc jazdy +/- 100, +przod, -tyl*/	

void gyroStraight(signed short ticks, signed short speed) {
	int PID = 0;
	int prop = 0, integ = 0;
	gyroOffset();
	
	while(1) {
		if(gyro_ready) {
			prop = - gyroGetHdg();
			integ += prop;
			PID = ((Kp_straight * prop) + (Ki_straight * integ));
			setTracks(speed + PID, speed - PID);
			gyro_ready = 0;
			ticks--;
			if(ticks <= 0) {
				setTracks(0, 0);
				break;
			}
		}
	}	
}

/** \fn void normal(signed short ticks, signed short speed)
		\brief Funkcja do jazdy robota na wprost bez stabilizacji (w celach porownawczych).
		\param ticks Czas jazdy * 2.5ms
		\param speed Szybkosc jazdy +/- 100, +przod, -tyl*/

void normal(signed short ticks, signed short speed) {	
	setTracks(speed, speed);
	while(1) {
		if(gyro_ready) {
			gyro_ready = 0;
			ticks--;
			if(ticks <= 0) {
				setTracks(0, 0);
				break;
			}
		}
	}
}	

/** \fn void setTracks(signed short left, signed short right)
		\brief Funkcja pozwalajaca ustawic predkosci gasienic przyjmujaca wartosci +/- 100.
		\param left Szybkosc jazdy lewej gasienicy +/- 100, +przod, -tyl
		\param right Szybkosc jazdy prawej gasienicy +/- 100, +przod, -tyl*/

void setTracks(signed short left, signed short right) {
	waitms(1);
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

/** \fn void waitms(unsigned short ms)
		\brief Funkcja sluzaca do czekania.
		\param ms Ilosc ms do odczekania*/

void waitms(unsigned short ms) {
	int inside, outside;	
	for(outside = 0; outside < ms; outside ++) {		
		for(inside = 0; inside < 8000; inside++);			
	}	
}
