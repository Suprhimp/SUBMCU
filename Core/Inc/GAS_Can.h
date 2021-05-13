/*
 * CAN.h
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

#include "stm32f4xx_hal_can.h"
#include "can.h"
#include "GAS_Vadc.h"
#include <stdio.h>
#include <stdlib.h>

typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		unsigned int isUpdated;
		unsigned int Sensor0 : 16;
		unsigned int Sensor1 : 16;

	}__attribute__((aligned(1),packed)) B;

}stm32_msg_t;



extern stm32_msg_t stm32_1;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage(uint8_t isUpdated, uint16_t Encoder1, uint16_t Encoder2);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
