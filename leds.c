#include "leds.h"												

const uint32_t ledGreen = 1UL<<5;			//Red led is Port D bit 5
const uint32_t ledRed = 1UL<<29;			//Green led is Port C bit 29

void ledsInitialize(void) {
	
	SIM->SCGC5 |=  (SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
	PORTD->PCR[5] = PORT_PCR_MUX(1); 
	PORTE->PCR[29] = PORT_PCR_MUX(1);
	FPTD->PSOR = ledGreen	;
	FPTE->PSOR = ledRed	;
	FPTD->PDDR = ledGreen	;
	FPTE->PDDR = ledRed	;
	
}
	
void ledsOn (void) {
	FPTD->PCOR |= ledGreen;
	FPTE->PCOR |= ledRed;
}

void ledsOff (void) {
	FPTD->PSOR |= ledGreen;
	FPTE->PSOR |= ledRed;
}

void ledGreenOn (void) {
	FPTD->PCOR |= ledGreen;
}

void ledRedOn (void) {
	FPTE->PCOR |= ledRed;
}

void ledGreenOff (void) {
	FPTD->PSOR |= ledGreen;
}

void ledRedOff (void) {
	FPTE->PSOR |= ledRed;
}

void ledGreenToggle (void) {
	FPTD->PTOR |= ledGreen;
}

void ledRedToggle (void) {
	FPTE->PTOR |= ledRed;
}

void welcome(void) {
	
	ledGreenOff();
	waitms(200);
	ledGreenOn();
	waitms(500);
	ledGreenOff();
	setTracksDir(REVERSE, REVERSE);
	setTracksSpeed(15, 15);
	waitms(100);
	ledGreenOn();
	setTracksDir(FORWARD, FORWARD);
	setTracksSpeed(15, 15);
	waitms(100);
	ledGreenOff();
	setTracksSpeed(0, 0);
	
	waitms(100);
	ledGreenOn();
	
}
