/*
 * GPIO_priv.h
 *
 *  Created on: 13 jul. 2021
 *      Author: lucky
 */

#ifndef GPIO_GPIO_PRIV_H_
#define GPIO_GPIO_PRIV_H_


#include "Prototype.h"

#define GPIO_A_PCR						((uint32*)0x40049000)
#define GPIO_B_PCR						((uint32*)0x4004A000)
#define GPIO_C_PCR						((uint32*)0x4004B000)
#define GPIO_D_PCR						((uint32*)0x4004C000)
#define GPIO_E_PCR						((uint32*)0x4004D000)

#define GPIO_A_MEM_MAP_ADDR				((uint32*)0x400FF000)
#define GPIO_B_MEM_MAP_ADDR				((uint32*)0x400FF040)
#define GPIO_C_MEM_MAP_ADDR				((uint32*)0x400FF080)
#define GPIO_D_MEM_MAP_ADDR				((uint32*)0x400FF0C0)
#define GPIO_E_MEM_MAP_ADDR				((uint32*)0x400FF100)

#define GPIO_CONGIF_REG_ADDRS			{	{GPIO_A_PCR, GPIO_A_MEM_MAP_ADDR},		\
											{GPIO_B_PCR, GPIO_B_MEM_MAP_ADDR},		\
											{GPIO_C_PCR, GPIO_C_MEM_MAP_ADDR},		\
											{GPIO_D_PCR, GPIO_D_MEM_MAP_ADDR},		\
											{GPIO_E_PCR, GPIO_E_MEM_MAP_ADDR} 		\
										}

#define LTR_LOC							(0U)
#define NUM_LOC							(1U)
#define SET_GPIO_ALT					(1U)
#define ONE_CHAR						(1U)
#define MIN_STR_LEN						(2U)
#define MAX_STR_LEN						(3U)
#define NUMBERS_OF_PINS					(32U)

#define SCGC5							(*(uint32*)0x40048038)
#define PORT_A_BIT						(9U)
#define PORT_B_BIT						(10U)
#define PORT_C_BIT						(11U)
#define PORT_D_BIT						(12U)
#define PORT_E_BIT						(13U)
#define PORTS_BASE_BIT					PORT_A_BIT

typedef enum
{
	GPIO_enPortA = 0,
	GPIO_enPortB,
	GPIO_enPortC,
	GPIO_enPortD,
	GPIO_enPortE,
	GPIO_enTotalPort
}GPIO_tenPortList;

typedef struct
{
	uint8 cAdvCfg;
	uint8 cPinMux;
	uint8 cInterruptConf;
	uint8 cInterrupFlaf;
}GPIO_tstPCRSettings;

typedef struct
{
	GPIO_tstPCRSettings astPCR[32];
	uint32 dwGPCLR;
	uint32 dwGPCHR;
	uint32 dwISFR;
}GPIO_tstPCR;

typedef struct
{
	uint32 dwPDOR;
	uint32 dwPSOR;
	uint32 dwPCOR;
	uint32 dwPTOR;
	uint32 dwPDIR;
	uint32 dwPDDR;
}GPIO_tstGPIOMemMap;


typedef struct
{
	uint32* p2PcrRegs;
	uint32* p2MemMapRegs;
}GPIO_tstConfRegisters;

void GPIO_vfnpinMode(uint8* pcPin, uint8 cDir, uint8 cStrLen);
void GPIO_vfndigitalWrite(uint8* pcPin, uint8 cState, uint8 cStrLen);
uint8 GPIO_cfndigitalRead(uint8* pcPin, uint8 cStrLen);


#endif /* GPIO_GPIO_PRIV_H_ */
