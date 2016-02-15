/** \file rgb.h
		\brief Funkcje obslugi diod LED (nie sa konieczne do poprawnego dzialania funkcji zwiazanych z zyroskopem)*/

#include "rgb.h"

void rgbInitialize() {
	
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	TPM1->SC &= ~TPM_SC_CPWMS_MASK;
	TPM1->SC |= TPM_SC_PS(7);
	TPM1->MOD = 0x8FF;
	TPM2->SC &= ~TPM_SC_CPWMS_MASK;
	TPM2->SC |= TPM_SC_PS(7);
	TPM2->MOD = 0x8FF;
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM1->CONTROLS[1].CnSC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
	TPM1->CONTROLS[1].CnV = 0;
	PORTE->PCR[21] |= PORT_PCR_MUX(3);
	TPM2->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM2->CONTROLS[0].CnSC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
	TPM2->CONTROLS[0].CnV = 0;
	PORTE->PCR[22] |= PORT_PCR_MUX(3);
	TPM1->SC |= TPM_SC_CMOD(1);
	TPM2->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM2->CONTROLS[1].CnSC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
	TPM2->CONTROLS[1].CnV = 0;
	PORTE->PCR[23] |= PORT_PCR_MUX(3);
	TPM2->SC |= TPM_SC_CMOD(1);
	
}
 
uint8_t brightness = 100;

void RGB(uint8_t r, uint8_t g, uint8_t b) {
	TPM2->CONTROLS[1].CnV = 0x8FF * r * brightness / 25500;
	TPM1->CONTROLS[1].CnV = 0x8FF * g * brightness / 25500;
	TPM2->CONTROLS[0].CnV = 0x8FF * b * brightness / 25500;
}

void rgbR(uint8_t r) {
	TPM2->CONTROLS[1].CnV = 0x8FF * r * brightness / 25500;
}

void rgbG(uint8_t g) {
	TPM1->CONTROLS[1].CnV = 0x8FF * g * brightness / 25500;
}

void rgbB(uint8_t b) {
	TPM2->CONTROLS[0].CnV = 0x8FF * b * brightness / 25500;
}

void rgbSetBrightness(uint8_t bright) {
	brightness = bright;
}

void rgbNavLitesInitialize() {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	PORTE->PCR[30] |= PORT_PCR_MUX(3);
	FPTB->PDDR |= (1UL << 18);
	FPTB->PCOR |= (1UL << 18);
	TPM0->CONTROLS[3].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
	TPM0->CONTROLS[3].CnSC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[3].CnV = 200;
}

void rgbStrobe(void) {
	static uint8_t count;
	static uint16_t stack;
	if(count == 0) 			 {RGB(255, 255, 255);}
	else if(count == 6)  {RGB(  0,   0,   0);}
	else if(count == 106) {RGB(255, 255, 255);}
	else if(count == 110) {RGB(  0,   0,   0);}
	else if(count == 150) {RGB(255, 255, 255);}
	else if(count == 154) {RGB(  0,   0,   0);}
	stack++;
	count = stack / 2;
	if(count > 255) stack = 0;
}

void rgbRedBlue(void) {
	static uint8_t count, delay, init;
	static uint16_t stack;
	if(!init) {
		RGB(0, 0, 0);
		init = 1;
	}
	if		 (count == 0) {rgbR(255);}
	else if(count == 5) {rgbR(0);}
	else if(count == 35) {rgbR(255);}
	else if(count == 40) {rgbR(0);}
	if		 (count == 120 - delay) {rgbB(255);}
	else if(count == 125 - delay) {rgbB(0);}
	else if(count == 155 - delay) {rgbB(255);}
	else if(count == 160 - delay) {rgbB(0);}
	stack++;
	count = stack / 2;
	if(count > 160) {
		stack = 0;
		delay += 10;
	}
	if(delay > 80) delay = 0;
}
