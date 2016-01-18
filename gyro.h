#ifndef gyro_h
#define gyro_h

#include "MKL46Z4.h"
#include "i2c1.h"

//Rejestry zyroskopu

#define GYR_CTRL1			0x20
#define GYR_CTRL2			0x21
#define GYR_CTRL3			0x22
#define GYR_CTRL4			0x23
#define GYR_CTRL5			0x24
#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29
#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B
#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D

void gyro_init(void);
static void pause(void);

uint8_t gyro_read_reg(uint8_t addr);
void gyro_write_reg(uint8_t addr, uint8_t data);

short iGyrReadX(void);
short iGyrReadY(void);
short iGyrReadZ(void);
signed short iGyrRead_avg(int, char);

#endif //__I2C_H__
