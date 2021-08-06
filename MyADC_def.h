/*
 * MyADC_def.h
 *
 *  Created on: 26 jul. 2021
 *      Author: lucky
 */

#ifndef MYADC_DEF_H_
#define MYADC_DEF_H_

#include "Prototype.h"

#define SCGC6								 (*(uint32*)0x4004803CU)
#define SCGC5								 (*(uint32*)0x40048038U)

#define ADC0_ADRRS							((uint32*)0x4003B000)
/* SCGC6 macros */
#define ENABLE_ADC_CLOCK					((uint32)(1 << 27))

/* SCGC5 macros */
#define ENABLE_PORT_E						(1 << 13)

/* SC1n macros */
#define DIFF_MODE_ENABLE					(1 << 5)
#define INTERRUPT_ENABLE					(1 << 6)
#define CONVERSION_COMPLETE					(1 << 7)

/* SC2 macros */
#define HARDWARE_TRIGGER					(1 << 6)

/* SC3 macros */
#define CONTINUOUS_CONVERSION				(1 << 3)
#define HW_AVERAGE_ENABLE					(1 << 2)
#define SAMPLE_AVERAGE_4					0U
#define SAMPLE_AVERAGE_8					1U
#define SAMPLE_AVERAGE_16					2U
#define SAMPLE_AVERAGE_32					3U

/* CFG1 macros */
#define DEFAULT_CFG							(0U)
#define RESOLUTION_8BITS					(0 << 2)
#define RESOLUTION_10BITS					(2 << 2)
#define RESOLUTION_12BITS					(1 << 2)
#define RESOLUTION_16BITS					(3 << 2)

typedef struct
{
	uint32 SC1A;
	uint32 SC1B;
	uint32 CFG1;
	uint32 CFG2;
	uint32 RA;
	uint32 RB;
	uint32 CV1;
	uint32 CV2;
	uint32 SC2;
	uint32 SC3;
}ADC0_tstType;

#endif /* MYADC_DEF_H_ */
