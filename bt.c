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
	NVIC_SetPriority (UART2_IRQn, 0);
	
}

uint8_t btdata;
extern int drive, spd;

void UART2_IRQHandler() {
		
	if(UART2->S1 & UART_S1_RDRF_MASK) {
		btdata = UART2->D;	
		switch(btdata) {
			case 'd':
				if(drive) drive = 0;
				else drive = 1;
				spd = 0;
				break;
		}
	}
	
}

int translateAN(int ascii) {
	switch(ascii) {
		case 48:
			return 0;
		case 49:
			return 1;
		case 50:
			return 2;
		case 51:
			return 3;
		case 52:
			return 4;
		case 53:
			return 5;
		case 54:
			return 6;
		case 55:
			return 7;
		case 56:
			return 8;
		case 57:
			return 9;
	}
	return 10; //error code
}

int translateNA(int ascii) {
	switch(ascii) {
		case 0:
			return 48;
		case 1:
			return 49;
		case 2:
			return 50;
		case 3:
			return 51;
		case 4:
			return 52;
		case 5:
			return 53;
		case 6:
			return 54;
		case 7:
			return 55;
		case 8:
			return 56;
		case 9:
			return 57;
	}
	return 0; //error code
}

void waitms(int ms) {
	int inside, outside;	
	for(outside = 0; outside < ms; outside ++) {		
		for(inside = 0; inside < 8000; inside++);			
	}	
}

void sendShort(signed short data) {
	
	char tab[7];
	int i;
	
	if(data >= 0) {
		tab[0] = 0x20;
	}
	else {
		tab[0] = '-';
		data = -data;
	}
	tab[6] = 0x0A;
	for(i = 5; i > 0; i--) {
				tab[i] = translateNA(data % 10);
				data = (data - (data % 10)) / 10;
			}
	for(i = 0; i < 7; i++) {
		while(!(UART2->S1 & UART_S1_TDRE_MASK));
		UART2->D = tab[i];
	}
		
}
