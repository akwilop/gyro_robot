#include "MKL46Z4.h"
#include "rgb.h"
#include "gyroDriver.h"

#define READY			0xFA
#define BUSY			0xFB
#define COMPLETE	0xFC

void btInitialize(void);
void UART2_IRQHandler(void);
void send(int, int);
