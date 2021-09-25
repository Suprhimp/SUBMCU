/*
 * GAS_Shock.h
 *
 *  Created on: 2021. 5. 16.
 *      Author: Suprhimp
 */

#ifndef INC_GAS_SHOCK_H_
#define INC_GAS_SHOCK_H_

#include "GAS_CAN.h"
#include "GAS_PWM.h"
#include "stm32f4xx_hal_can.h"
#include "can.h"

typedef struct{
	int initAngleR;
	int initAngleL;
	unsigned int AngleR;
	unsigned int AngleL;
	float Roll;
	float Heave;
}ShockAngle;


extern ShockAngle angle;
extern void GAS_Shock_Run(unsigned int Angle1, unsigned int Angle2 ,int isUpdated);
extern void GAS_Shock_init(int angleR, int angleL);
#endif /* INC_GAS_SHOCK_H_ */
