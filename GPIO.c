/*
 * GPIO.c
 *
 *  Created on: 13 jul. 2021
 *      Author: lucky
 */

#include "Prototype.h"

#include "GPIO_priv.h"
#include "GPIO.h"

#define Check_bit(reg, bit)						(((uint32)reg) & ((uint32)(1 << bit)))
#define Set_bit(v, bit)							((uint32)(v << bit))

#define Port_clk_is_enable(port)				(Check_bit(SCGC5, (PORTS_BASE_BIT + port)))
#define Enable_Port_clk(port)					((uint32)(1 << ((PORTS_BASE_BIT + port))))

#define Enable_Gpio_Alt(port, pin)				((GPIO_tstPCR*)astGpioAddrs[port].p2PcrRegs)->astPCR[pin].cPinMux = (uint8)SET_GPIO_ALT
#define Is_gpio_enable(port, pin)				Check_bit(((GPIO_tstPCR*)astGpioAddrs[port].p2PcrRegs)->astPCR[pin].cPinMux, 0)

#define Set_Pin_Dir(port, pin)					((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPDDR |= Set_bit(1, pin)
#define Set_Pin_Dir_In(port, pin)				((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPDDR &= ~(Set_bit(1, pin))
#define Is_dir_out(port, pin)					Check_bit(((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPDDR, pin)

#define Clear_pin_output(port, pin)				((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPCOR = Set_bit(1, pin)
#define Set_pin_output(port, pin)				((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPSOR = Set_bit(1, pin)

#define Read_pin(port, pin)						(((((GPIO_tstGPIOMemMap*)astGpioAddrs[port].p2MemMapRegs)->dwPDIR) >> pin) & 1)


#define Minus_Letter_Char(x)					(x - 1)

#define Char_2_Int_Uni(x)						(x - '0')
#define Char_2_Int_Dec(x)						((x - '0') * 10)


static GPIO_tstConfRegisters astGpioAddrs[GPIO_enTotalPort] = GPIO_CONGIF_REG_ADDRS;

typedef struct
{
	uint8 cPort;
	uint8 cNumber;
}GPIO_tstPin;

uint8 cfnGetPort(uint8 cPort, uint8* pcSavePort)
{
	uint8 cSatus = N_OK;

	cPort -= 'A';

	if(cPort < (uint8)GPIO_enTotalPort)
	{
		*pcSavePort = cPort;
		cSatus = OK;
	}

	return cSatus;
}

uint8 cfnGetPinNum(uint8* pcStr, uint8* pcSaveNum, uint8 cStrLen)
{
	uint8 cSatus = N_OK;
	uint8 cPinNumber = 0U;

	if(cStrLen == (uint8)ONE_CHAR)
	{
		cPinNumber = Char_2_Int_Uni(pcStr[0]);
	}
	else
	{
		cPinNumber = Char_2_Int_Dec(pcStr[0]);
		cPinNumber += Char_2_Int_Uni(pcStr[1]);
	}

	if(cPinNumber < (uint8)NUMBERS_OF_PINS)
	{
		*pcSaveNum = cPinNumber;
		cSatus = OK;
	}

	return cSatus;
}

uint8 cfnCheckParams(uint8* pcPin, uint8 cStrLen, GPIO_tstPin* pstSavePin)
{
	uint8 cStatus = N_OK;

	if((cStrLen <= (uint8)MAX_STR_LEN) && (cStrLen >= (uint8)MIN_STR_LEN))
	{
		cStatus = cfnGetPort(pcPin[LTR_LOC], (uint8*)&pstSavePin->cPort);
		cStatus = cfnGetPinNum(&pcPin[NUM_LOC], (uint8*)&pstSavePin->cNumber, Minus_Letter_Char(cStrLen));
	}

	return cStatus;
}

void GPIO_vfnpinMode(uint8* pcPin, uint8 cDir, uint8 cStrLen)
{
	uint8 cStatus;
	GPIO_tstPin stPin2Set;

	cStatus = cfnCheckParams(pcPin, cStrLen, &stPin2Set);

	if(cStatus == (uint8)OK)
	{
		if(!Port_clk_is_enable(stPin2Set.cPort))
		{
			SCGC5 |= Enable_Port_clk(stPin2Set.cPort);
		}

		Enable_Gpio_Alt(stPin2Set.cPort, stPin2Set.cNumber);
		if(cDir == OUTPUT)
			Set_Pin_Dir(stPin2Set.cPort, stPin2Set.cNumber);
		else
			Set_Pin_Dir_In(stPin2Set.cPort, stPin2Set.cNumber);
	}
}

void GPIO_vfndigitalWrite(uint8* pcPin, uint8 cState, uint8 cStrLen)
{
	uint8 cStatus;
	GPIO_tstPin stPin2Set;

	cStatus = cfnCheckParams(pcPin, cStrLen, &stPin2Set);

	if(cStatus == (uint8)OK)
	{
		if((Port_clk_is_enable(stPin2Set.cPort)) && Is_gpio_enable(stPin2Set.cPort, stPin2Set.cNumber)
				&& (Is_dir_out(stPin2Set.cPort, stPin2Set.cNumber)))
		{
			if(cState == 0)
			{
				Clear_pin_output(stPin2Set.cPort, stPin2Set.cNumber);
			}
			else
			{
				Set_pin_output(stPin2Set.cPort, stPin2Set.cNumber);
			}
		}
	}
}

uint8 GPIO_cfndigitalRead(uint8* pcPin, uint8 cStrLen)
{
	uint8 cStatus;
	GPIO_tstPin stPin2Set;

	cStatus = cfnCheckParams(pcPin, cStrLen, &stPin2Set);

	if(cStatus == (uint8)OK)
	{
		if((Port_clk_is_enable(stPin2Set.cPort)) && Is_gpio_enable(stPin2Set.cPort, stPin2Set.cNumber)
				&& (!Is_dir_out(stPin2Set.cPort, stPin2Set.cNumber)))
		{
			cStatus = Read_pin(stPin2Set.cPort, stPin2Set.cNumber);
		}
	}

	return cStatus;
}
