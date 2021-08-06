/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ADC.c
 * @brief   Application entry point.
 */

#include "Prototype.h"
#include "MyADC.h"
#include "GPIO.h"

#define bool(x)					(x == 0)
#define TOTAL_PINS				4U
#define MAX_ADC_TOL				128U
#define DELAY_TIME 				1538U
#define ONE_SEC					790U
#define HALF_SEC				396U
#define TRIGGER_BTN			 	16U			///20ms

void vfnUpdatePinA(uint8 cState);
void vfnUpdatePinB(uint8 cState);
void vfnUpdatePinC(uint8 cState);
void vfnUpdatePinD(uint8 cState);
void vfnDelay(uint32 cnt);

static uint16 cCounter = 0;
static uint8 cAlarm = 0;
static uint8 cState = 0;
static uint8 cIndex = 0;
static uint8 cPrevIndex = 1;
static uint8 debouncerCnt = 0;
void (*p2avfn[TOTAL_PINS])(uint8) = {vfnUpdatePinA, vfnUpdatePinB, vfnUpdatePinC, vfnUpdatePinD};

void vfnUpdateSeq(void)
{
	p2avfn[cIndex](ON);
	p2avfn[cPrevIndex](OFF);

	cPrevIndex = cIndex;

	if(cState == 0)
	{
		if(cIndex == 3)
		{
			cState = 1;
			cIndex--;
		}
		else
		{
			cIndex++;
		}
	}
	else
	{
		if(cIndex == 0)
		{
			cState = 0;
			cIndex++;
		}
		else
		{
			cIndex--;
		}
	}
}

void vfnTurnOffSeq(void)
{
	uint8 i;
	for(i = 0; i < 4; i++)
	{
		p2avfn[i](0);
	}
	cIndex = 0;
	cPrevIndex = 1;
	cState = 0;
}

uint8 cfnDebouncer(void)
{
	uint8 OneStable = 0;
	uint8 btn = 0;

	btn = digitalRead(C0);

	if(btn == 1)
	{
		debouncerCnt++;
		if(debouncerCnt == 3)
		{
			debouncerCnt = 0;
			OneStable = 1;
		}
	}
	else
	{
		debouncerCnt = 0;
	}

	return OneStable;
}

int main(void) {
	uint16 adc;

	/* LEDS */
	pinMode(C3, OUTPUT);
	pinMode(C4, OUTPUT);
	pinMode(C5, OUTPUT);
	pinMode(C6, OUTPUT);

	/* Boton */
	pinMode(C0, INPUT);

	/* Inicializacion del ADC */
	ADC_vfnInit();

    while(1)
    {
    	vfnDelay(DELAY_TIME);
    	cCounter++;

    	if(cAlarm)
    	{
    		if(cCounter == HALF_SEC)
    		{
    			vfnUpdateSeq();
    			cCounter = 0;
    		}

    		if(cCounter % TRIGGER_BTN == 0)
    		{
    			if(cfnDebouncer() == 1)
    			{
    				vfnTurnOffSeq();
    				cAlarm = 0;
    			}
    		}
    	}
    	else
    	{
    		adc = ADC_wfnAnalogRead();

    		if(adc > MAX_ADC_TOL)
    		{
    			cAlarm = 1;
    		}
    		if(cCounter == HALF_SEC)
    		{
    			cCounter = 0;
    		}
    	}
    }
    return 0 ;
}

void vfnDelay(uint32 cnt)
{
	while(cnt != 0)
	{
		cnt--;
	}
}

void vfnUpdatePinA(uint8 cState)
{
	digitalWrite(C3, cState);
}

void vfnUpdatePinB(uint8 cState)
{
	digitalWrite(C4, cState);
}

void vfnUpdatePinC(uint8 cState)
{
	digitalWrite(C5, cState);
}

void vfnUpdatePinD(uint8 cState)
{
	digitalWrite(C6, cState);
}
