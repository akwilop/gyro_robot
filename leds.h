#ifndef leds_h
#define leds_h

#include "MKL46Z4.h"
#include "bt.h"
#include "motorDriver.h"

void ledsInitialize(void);

void ledsOn (void);
void ledsOff (void);
void ledGreenOn (void);
void ledRedOn (void);
void ledGreenOff (void);
void ledRedOff (void);
void ledGreenToggle (void);
void ledRedToggle (void);
void welcome(void);

#endif
