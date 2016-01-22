#ifndef RGB_H
#define RGB_H

#define R(x) TPM2->CONTROLS[0].CnV = x
#define G(x) TPM1->CONTROLS[1].CnV = x
#define B(x) TPM1->CONTROLS[0].CnV = x

#define RGB(r,g,b) {R(r); G(g); B(b);}

#include "MKL46Z4.h"

void rgbInitialize(void);

#endif
