/*
 * GAS_UART.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Suprhimp
 */

#ifndef INC_GAS_UART_H_
#define INC_GAS_UART_H_

#include "usart.h"
#include "stm32f4xx_hal_uart.h"
#define uartMsgSize 20

#define IDCODE 0b110
#define DATACODE1 0b1110
#define DATACODE2 0b10
#define DATACODE3 0b10



typedef struct{
	union{
		uint8_t bit8;
		struct{
			unsigned int idData : 5;
			unsigned int idCode : 3;
		}__attribute__((aligned(1),packed)) B;
	}ID;
	union{
		uint8_t bit8;
		struct{
			unsigned int data : 4;
			unsigned int dataCode1 : 4;
		}__attribute__((aligned(1),packed)) B;
	}data1;
	union{
		uint8_t bit8;
		struct{
			unsigned int data : 6;
			unsigned int dataCode2 : 2;
		}__attribute__((aligned(1),packed)) B;
	}data2;
	union{
		uint8_t bit8;
		struct{
			unsigned int data : 6;
			unsigned int dataCode3 : 2;
		}__attribute__((aligned(1),packed)) B;
	}data3;
}UartMsg_t;

typedef union{
	uint16_t Data;
	struct{
		unsigned int data3 : 6;
		unsigned int data2 : 6;
		unsigned int data1 : 4;
	}S_Data;
}UartMsgData_t;

extern UartMsg_t UartMsg[uartMsgSize];


extern void GAS_Uart_init(void);
extern void GAS_Uart_sendMessage(void);
#endif /* INC_GAS_UART_H_ */
