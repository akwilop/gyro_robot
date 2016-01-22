/** \file "mag2.h"
\brief Niskopoziomowa obsluga kompasu. */

#include "gyro.h"

signed short GyrResX, GyrResY, GyrResZ;

//#define MAG22_I2C2_ADDRESS (0x1D<<1) ///LSM303D
#define GYRO_I2C2_ADDRESS (0xD6) ///LSM303D

/** \brief Funkcja opozniajaca zapis i odczyt */
static void pause(void)
{    int n;
    for(n=0; n<40; n++){}
}

/**
	\brief Funkcja zwracajaca wartosc rejestru magnetometru
	\param addr Adres odczytywanego rejestru
	\return Odczytana wartosc rejestru magnetometru
*/
uint8_t gyro_read_reg(uint8_t addr)
{
    uint8_t result2;
	
	__disable_irq();
	
    i2c1_start();
    i2c1_write_byte(GYRO_I2C2_ADDRESS | I2C1_WRITE);
    
    i2c1_wait();
		i2c1_get_ack();

    i2c1_write_byte(addr);
    i2c1_wait();
		i2c1_get_ack();
	
    i2c1_repeated_start();
    i2c1_write_byte(GYRO_I2C2_ADDRESS | I2C1_READ);
    i2c1_wait();
		i2c1_get_ack();
	
    i2c1_set_rx_mode();

    i2c1_give_nack();
    result2 = i2c1_read_byte();
    i2c1_wait();
		i2c1_get_ack();

    i2c1_stop();
    result2 = i2c1_read_byte();
    pause();
		__enable_irq();
    return result2;
}

/**
	\brief Funkcja wpisujaca wartosc do rejestru magnetometru
	\param addr Adres rejestru do ktorego zostanie wpisana dana 
	\param data Dane wpisywane do rejestru
*/
void gyro_write_reg(uint8_t addr, uint8_t data)
{
	__disable_irq();
	
    i2c1_start();

    i2c1_write_byte( GYRO_I2C2_ADDRESS|I2C1_WRITE);
    i2c1_wait();
		i2c1_get_ack();

    i2c1_write_byte(addr);
    i2c1_wait();
		i2c1_get_ack();
	
		i2c1_write_byte(data);
    i2c1_wait();
		i2c1_get_ack();
	
    i2c1_stop();
	
	__enable_irq();
    pause();
}

/** \brief Inicjalizacja Mag2netometru */	
void gyro_init(void)
{
		hal_i2c1_init();         
    gyro_write_reg(GYR_CTRL1, 0x0C);
}

/** \brief Funkcja zwracajaca wartosc osi X */	
signed short iGyrReadX(void) {

      GyrResX   = gyro_read_reg(GYR_OUT_X_H)<<8;
      GyrResX  |= gyro_read_reg(GYR_OUT_X_L);

    return GyrResX;
}

/** \brief Funkcja zwracajaca wartosc osi Y */	
signed short iGyrReadY(void) {

      GyrResY   = gyro_read_reg(GYR_OUT_Y_H)<<8;
      GyrResY  |= gyro_read_reg(GYR_OUT_Y_L);      

    return GyrResY;
}

/** \brief Funkcja zwracajaca wartosc osi Z */	
signed short iGyrReadZ(void) {

      GyrResZ   = gyro_read_reg(GYR_OUT_Z_H)<<8;	
      GyrResZ  |= gyro_read_reg(GYR_OUT_Z_L);
	
    return GyrResZ;
}

signed short iGyrRead_avg(int samples, char axis) {
	
	int j; signed long int avg = 0;

    for(j = 0; j < samples; j++) {
			switch(axis) {
				case 'x':
					avg += iGyrReadX();
					break;
				case 'y':
					avg += iGyrReadY();
					break;
				case 'z':
					avg += iGyrReadZ();
					break;
			}
		}			
	
		avg = avg / samples;
		
    return avg;
}
