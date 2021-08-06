/*
 * MyADC.c
 *
 *  Created on: 26 jul. 2021
 *      Author: lucky
 */


#include "Prototype.h"

#include "MyADC_def.h"
#include "MyADC.h"

#define DIFF_MODE_DISABLE(adc)					(((ADC0_tstType*)adc)->SC1A &= ~(DIFF_MODE_ENABLE))
#define INTERRUPT_DISABLE(adc)					(((ADC0_tstType*)adc)->SC1A &= ~(INTERRUPT_ENABLE))
#define SOFTWARE_TRIGGER(adc)					(((ADC0_tstType*)adc)->SC2 &= ~(HARDWARE_TRIGGER))
#define CHANNEL_0								0x1F

void ADC_vfnInit(void)
{
	ADC0_tstType* ADC0 = (ADC0_tstType*)ADC0_ADRRS;
	SCGC6 |= ENABLE_ADC_CLOCK;
	SCGC5 |= ENABLE_PORT_E;

	DIFF_MODE_DISABLE(ADC0);
	INTERRUPT_DISABLE(ADC0);

	ADC0->CFG1 = (DEFAULT_CFG + RESOLUTION_8BITS);
	ADC0->CFG2 = DEFAULT_CFG;

	SOFTWARE_TRIGGER(ADC0);

	ADC0->SC3 |= CONTINUOUS_CONVERSION;

}

uint16 ADC_wfnAnalogRead(void)
{
	ADC0_tstType* ADC0 = (ADC0_tstType*)ADC0_ADRRS;

	ADC0->SC1A &= ~(CHANNEL_0);

	while(!(ADC0->SC1A & CONVERSION_COMPLETE));

	return ADC0->RA;
}
