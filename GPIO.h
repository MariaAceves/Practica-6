/*
 * GPIO.h
 *
 *  Created on: 13 jul. 2021
 *      Author: lucky
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include "Prototype.h"

#include "GPIO_priv.h"

#define OFF				(0U)
#define OUTPUT			(0U)
#define INPUT			(1U)
#define ON				(1U)

#define pinMode(pin, dir)			GPIO_vfnpinMode((uint8*)#pin, dir, sizeof(#pin) - 1)
#define digitalWrite(pin, state)	GPIO_vfndigitalWrite((uint8*)#pin, state, sizeof(#pin) - 1)
#define digitalRead(pin)			GPIO_cfndigitalRead((uint8*)#pin, sizeof(#pin) - 1)


#endif /* GPIO_GPIO_H_ */
