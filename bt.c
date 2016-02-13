/** \file bt.h
		\brief Funkcje obslugi komunikacji Blueooth*/

#include "bt.h"

void btInitialize() {

	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[16] = PORT_PCR_MUX(3);
	PORTE->PCR[17] = PORT_PCR_MUX(3);
	UART2->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
	UART2->BDH &= ~UART_BDH_SBNS_MASK;
	UART2->BDH |= UART_BDH_SBR(0);
	UART2->BDL = UART_BDL_SBR(156);
	UART2->C1 &= ~(UART_C1_M_MASK | UART_C1_PE_MASK);
	UART2->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK | UART_C2_RIE_MASK);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_SetPriority(UART2_IRQn, 1);
		
}

signed short int time, speed, hdg;
uint8_t state = READY;

void UART2_IRQHandler() {
	if(UART2->S1 & UART_S1_RDRF_MASK) {
		switch(UART2->D) {
			case 's':
			case 'S':
				NVIC_DisableIRQ(UART2_IRQn);
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				time = (UART2->D - '0') * 10;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				time += (UART2->D - '0');
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed = (UART2->D - '0') * 100;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed += (UART2->D - '0') * 10;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed += (UART2->D - '0');
				speed -= 100;
				state = BUSY;
				gyroStraight(time * 40, speed);
				state = COMPLETE;
				waitms(500);
				state = READY;
				break;
			case 'n':
			case 'N':
				NVIC_DisableIRQ(UART2_IRQn);
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				time = (UART2->D - '0') * 10;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				time += (UART2->D - '0');
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed = (UART2->D - '0') * 100;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed += (UART2->D - '0') * 10;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				speed += (UART2->D - '0');
				speed -= 100;
				state = BUSY;
				normal(time * 40, speed);	
				state = COMPLETE;
				waitms(500);
				state = READY;			
				break;
			case 't':
			case 'T':
				NVIC_DisableIRQ(UART2_IRQn);
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				hdg = (UART2->D - '0') * 100;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				hdg += (UART2->D - '0') * 10;
				while(!(UART2->S1 & UART_S1_RDRF_MASK));
				hdg += (UART2->D - '0');
				hdg = (hdg - 500) * 10;
				state = BUSY;
				gyroTurn(hdg);
				state = COMPLETE;
				waitms(500);
				state = READY;
				break;
			case 'h':
			case 'H':
				NVIC_DisableIRQ(UART2_IRQn);
				send(gyroGetHdg(), 4);
		}	
	NVIC_EnableIRQ(UART2_IRQn);		
	}
}

void send(int data, int digits) {
	
	char tab[12];
	int i;
	if (digits > 6) digits = 6;
	
	if(data >= 0) {
		tab[0] = 0x20;
	}
	else {
		tab[0] = '-';
		data = -data;
	}
	tab[digits + 1] = 0x0A;
	for(i = digits; i > 0; i--) {
				tab[i] =(data % 10) + '0';
				data = (data - (data % 10)) / 10;
			}
	for(i = 0; i < digits + 2; i++) {
		while(!(UART2->S1 & UART_S1_TDRE_MASK));
		UART2->D = tab[i];
	}
		
}
