/*
 * GAS_PWM.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#include "GAS_PWM.h"
#include "stdio.h"

pwmIn_t pwmIn1;
pwmIn_t pwmIn2;


void GAS_PWM_inputInit(void);
void GAS_PWM_outputInit(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void GAS_PWM_changeOutput_ch1(uint16_t input);


void GAS_PWM_inputInit(void)
{

	HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2);
}

void GAS_PWM_outputInit(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	/*
	 * For PWM input function
	 * One PWM input per one htim channel(because of TIM counter)
	 * Period, width, duty cycle, frequency can be calculated.
	 */
	if(htim ->Instance == TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			pwmIn1.RisingEdgeValue = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			TIM8->CNT = 0;
			pwmIn1.Period=pwmIn1.RisingEdgeValue;
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			pwmIn1.FallingEdgeValue=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			pwmIn1.Width=pwmIn1.FallingEdgeValue;
			pwmIn1.DutyCycle = 100*pwmIn1.Width/pwmIn1.Period;
			pwmIn1.Frequency= 1000000/pwmIn1.Period; //1sec=10^6us
		}
	}
}

void GAS_PWM_changeOutput_ch1(uint16_t input)
{
	/*
	 * PWM change duty cycle function
	 * Change duty cycle by changing register CCR directly
	 */
	TIM1->CCR1=input/10;
}
