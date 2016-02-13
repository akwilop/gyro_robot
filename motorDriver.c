/** \file motorDriver.h
		\brief Funkcje obslugi silnikow (wykorzystywane w bibliotece gyroDriver)*/

#include "motorDriver.h"
#include "math.h"
//////////////////////////////////////// Global variables initialization ////////////////////////////////////////



// drive way and encoder turning functionality variables
uint32_t actualDistance = 0;
uint8_t flagLimitDistance = 0;
uint32_t limit = 0;

// compass turning functionality variables
uint8_t flagLimitAngle = 0;
uint32_t limitAngle = 0;
uint32_t offset = 0;

// distance counter functionality variables
uint32_t countersDistance = 0;
uint8_t flagCounting = 0;
uint8_t flagTurning = 0;

// turning speed
uint32_t turningSpeed = DEFAULT_TURNING_SPEED;

uint16_t azimuth = 0;

uint16_t tmp = 0;

//////////////////////////////////////// Functions ////////////////////////////////////////

void motorDriverInit(void)
{
	 // CLOCK_SETUP 1
	 //1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
   //      Reference clock source for MCG module is an external crystal 8MHz
   //      Core clock = 48MHz, BusClock = 24MHz
	
	// connect the clock
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK
							| SIM_SCGC5_PORTC_MASK
							|	SIM_SCGC5_PORTD_MASK;
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	
	// choose signals for pins
	PORTA->PCR[13]|= PORT_PCR_MUX(1); // PHASE - Left
	PORTC->PCR[9] |= PORT_PCR_MUX(1); // PHASE - Right
	PORTD->PCR[2] |= PORT_PCR_MUX(4); // TPM0_CH2 - PWM - Right
	PORTD->PCR[4] |= PORT_PCR_MUX(4); // TPM0_CH4 - PWM - Left
	
	
	// set PHASE pins (GPIO) as output
	PTA->PDDR |= (1uL<<13);
	PTC->PDDR |= (1uL<<9);
	
	
	//////////////////////////////////////// PWM ////////////////////////////////////////
	
	//select clock for TMP0
	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // set 'MCGFLLCLK clock or MCGPLLCLK/2'
	
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // set "MCGPLLCLK clock with fixed divide by two"
	
	// set "up-counting"
	TPM0->SC &= ~TPM_SC_CPWMS_MASK; // default set
	
	// divide by 1
	TPM0->SC &= ~TPM_SC_PS_MASK; // the same TPM_SC_PS(0)
	//TPM0->SC |=  TPM_SC_PS(6);
	
	// clear counter
	TPM0->CNT = 0x00; 
	
	// set MOD for PWM period equal 4095 ( 12 bit)
	TPM0->MOD = V_MOD;
	//TPM0->MOD = 60000;
	////////////////////////////////////////////////////
	// Enable general TPM0 interrupt
	//TPM0->SC |= TPM_SC_TOIE_MASK;
	
	
	//SET TPM1 as not so fast timer
	
  /*SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	TPM1->SC &= ~TPM_SC_CPWMS_MASK;
	TPM1->SC |= TPM_SC_PS(7);
	TPM1->CNT = 0x00; 
	TPM1->MOD = 1000;
	NVIC_ClearPendingIRQ(TPM1_IRQn);
	NVIC_EnableIRQ(TPM1_IRQn);	
	NVIC_SetPriority(TPM1_IRQn,1);
	TPM1->SC |= TPM_SC_CMOD(1);
	TPM1->SC &= ~TPM_SC_TOIE_MASK;*/
	
	
	//////////////////////////////////////// ENGINE CHANNELS ////////////////////////////////////////
	
	
	//Right engine
	// set TPM0 channel 2 - "Edge-aligned PWM High-true pulses"
	TPM0->CONTROLS[2].CnSC |= TPM_CnSC_MSB_MASK	
												 |  TPM_CnSC_ELSB_MASK;
													
	// Default value for Right engine
	TPM0->CONTROLS[2].CnV = 0; // STOP
	
	
	//Left engine
	// set TPM0 channel 4 - "Edge-aligned PWM High-true pulses"
	TPM0->CONTROLS[4].CnSC |= TPM_CnSC_MSB_MASK	
												 |  TPM_CnSC_ELSB_MASK;
													
	// Default value for Left engine
	TPM0->CONTROLS[4].CnV = 0; // STOP
	
	NVIC_ClearPendingIRQ(TPM0_IRQn);				/* Clear NVIC any pending interrupts on TPM0 */
	NVIC_EnableIRQ(TPM0_IRQn);							/* Enable NVIC interrupts source for TPM0 module */
	
	NVIC_SetPriority (TPM0_IRQn, PRIORITY_INTERRUPT_DRIVE);	// priority interrupt
	
	// enable counter
	TPM0->SC |= TPM_SC_CMOD(1);
	
}



void setLeftTrackDir(int direction)
{
	
	if(direction == FORWARD)
	{
		PTA->PCOR |= LEFT_PHASE_PIN;
	}
	else if(direction == REVERSE)
	{
		PTA->PSOR |= LEFT_PHASE_PIN;
	}
	
}


void setLeftTrackSpeed(int speed)
{
	
	if( speed >= 0 )
	{
		if( speed < 100 )
		{
			TPM0->CONTROLS[4].CnV = V_MOD * speed/100;
		}
		else if ( speed == 100 )
		{
			TPM0->CONTROLS[4].CnV = V_MOD + 1;
		}
	}
	
}


void setRightTrackDir(int direction)
{
	
	if(direction == FORWARD)
	{
		PTC->PCOR |= RIGHT_PHASE_PIN;
	}
	else if(direction == REVERSE)
	{
		PTC->PSOR |= RIGHT_PHASE_PIN;
	}
	
}


void setRightTrackSpeed(int speed)
{
	
	if( speed >= 0 )
	{
		if( speed < 100 )
		{
			TPM0->CONTROLS[2].CnV = V_MOD * speed/100;
		}
		else if ( speed == 100 )
		{
			TPM0->CONTROLS[2].CnV = V_MOD + 1;
		}
	}
	
}


void setTracksDir(int leftDir, int rightDir)
{
	
	setLeftTrackDir( leftDir );
	setRightTrackDir( rightDir );
	
}


void setTracksSpeed(int leftSpeed, int rightSpeed)
{
	if (leftSpeed>100)leftSpeed=100;
	if (rightSpeed>100)rightSpeed=100;
	setLeftTrackSpeed( leftSpeed );
	setRightTrackSpeed( rightSpeed );
	
}
