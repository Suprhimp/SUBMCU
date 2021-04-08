/*
 * GAS_PWM.h
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#ifndef INC_GAS_PWM_H_
#define INC_GAS_PWM_H_

#include "tim.h"
#include "stm32f4xx_hal_tim.h"
typedef struct{
	uint32_t RisingEdgeValue;
	uint32_t FallingEdgeValue;
	uint32_t Period;
	uint32_t Width;
	uint32_t DutyCycle;
	uint32_t Frequency;
}pwmIn_t;

extern pwmIn_t pwmIn1;
extern pwmIn_t pwmIn2;

extern void GAS_PWM_inputInit(void);
extern void GAS_PWM_outputInit(void);
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
extern void GAS_PWM_changeOutput_ch1(uint16_t input);

#endif /* INC_GAS_PWM_H_ */
