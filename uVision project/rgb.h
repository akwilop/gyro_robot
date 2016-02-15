#ifndef RGB_H
#define RGB_H

#include "MKL46Z4.h"
#include "gyroDriver.h"

void RGB(uint8_t, uint8_t, uint8_t);
void rgbR(uint8_t);
void rgbG(uint8_t);
void rgbB(uint8_t);
void rgbSetBrightness(uint8_t);
void rgbInitialize(void);
void rgbNavLitesInitialize(void);
void rgbStrobe(void);
void rgbRedBlue(void);


#endif
