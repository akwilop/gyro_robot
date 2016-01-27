#include "MKL46Z4.h"

void btInitialize(void);
void UART2_IRQHandler(void);
int translateAN(int);
int translateNA(int);

void send(int, int);
