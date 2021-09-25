/*
 * GAS_PWM.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 *
 *      must calibrate the SeraRisingTime and Maxwidth by debugging.
 *      Check the Falling edge to find them!!!
 *      pwm1->R, pwm2->L
 */

#include "GAS_PWM.h"
#include "stdio.h"

#define SeraRisingTime 267
#define SeraMaxWidth 8335

#define SeraRisingTime2 269
#define SeraMaxWidth2 8390


pwmIn_t pwmIn1;
pwmIn_t pwmIn2;
pwmIn_t pwmIn3;

volatile uint8_t pwmChangeFlag = 0;

void GAS_PWM_inputInit(void);
void GAS_PWM_outputInit(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void GAS_PWM_changeOutput_ch1(uint16_t input);
void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn);

void GAS_PWM_inputInit(void)
{

	HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
//	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
//	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
}

void GAS_PWM_outputInit(void)
{
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

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
		GAS_PWM_Check2(htim, &pwmIn1);
		pwmChangeFlag = 1;
	}
	else if(htim ->Instance == TIM1){
		GAS_PWM_Check(htim, &pwmIn2);
		pwmChangeFlag = 1;
	}
//	if(htim ->Instance == TIM4){
//		GAS_PWM_Check2(htim, &pwmIn3);
//		pwmChangeFlag = 1;
//	}
}

void GAS_PWM_changeOutput_ch1(uint16_t input)
{
	/*
	 * PWM change duty cycle function
	 * Change duty cycle by changing register CCR directly
	 */
	TIM1->CCR1=input/10;
}

void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn){

			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				pwmIn->RisingEdgeValue = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
//				TIM1->CNT = 0;
				htim->Instance->CNT = 0;
				pwmIn->Period=pwmIn->RisingEdgeValue;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				pwmIn->FallingEdgeValue=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				pwmIn->Width=pwmIn->FallingEdgeValue;
				pwmIn->angle = (3600*(pwmIn->Width-SeraRisingTime)/(SeraMaxWidth-SeraRisingTime));
				if(pwmIn->angle>=3600)pwmIn->angle=0;

			}

}

void GAS_PWM_Check2(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn){

			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				pwmIn->RisingEdgeValue = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				htim->Instance->CNT = 0;
				pwmIn->Period=pwmIn->RisingEdgeValue;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				pwmIn->FallingEdgeValue=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				pwmIn->Width=pwmIn->FallingEdgeValue;
				pwmIn->angle = (3600*(pwmIn->Width-SeraRisingTime2)/(SeraMaxWidth2-SeraRisingTime2));
				if(pwmIn->angle>=3600)pwmIn->angle=0;

			}

}
