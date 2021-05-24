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
	int AngleR;
	int AngleL;
	float parsedAngleR;
	float parsedAngleL;
}ShockAngle;


extern ShockAngle angle;
extern void GAS_Shock_Run(pwmIn_t *pwmIn1, pwmIn_t *pwmIn2 ,int isUpdated);

#endif /* INC_GAS_SHOCK_H_ */
