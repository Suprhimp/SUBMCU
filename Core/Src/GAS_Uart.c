/*
 * GAS_UART.c
 *
 *  Created on: Jan 2, 2021
 *      Author: Suprhimp
 */
#include <GAS_Uart.h>
#include <stdio.h>

uint8_t uartData;
uint16_t RecieveData[uartMsgSize];
volatile uint8_t WriteCount = 0;

uint32_t ErrorCount[2];
uint8_t ReadCount=0;

UartMsg_t UartMsg[uartMsgSize];
uint8_t R_ID;
UartMsgData_t R_Data;

void GAS_Uart_init(void);
void GAS_Uart_sendMessage(void);
UartMsg_t GAS_Uart_encode(uint8_t id, uint16_t data);
void GAS_Uart_decode(uint8_t inputdata);


void GAS_Uart_init(void){
	HAL_UART_Receive_IT(&huart1, &uartData,1);
}
void GAS_Uart_sendMessage(void){
	/*
	 * Uart send function
	 * Can send 16bits of 20 data at once.
	 * HAL_UART_Receive_IT after send message.
	 * Check structure of UartMsg_t. It consists of ID and data
	 * USE!!! GAS_Uart_encode function for encode data!!!
	 */
//	for(int i=0;i<20;i++){
//		UartMsg[i] = GAS_Uart_encode(ID, Data);
//	}
	HAL_UART_Transmit(&huart1, (uint8_t*)UartMsg, sizeof(UartMsg), HAL_MAX_DELAY);
	HAL_UART_Receive_IT(&huart1, &uartData,1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/*
	 * Uart receive interrupt function
	 * USE!! GAS_Uart_decode function after receive uart data.
	 */
	if(huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, &uartData,1);
		GAS_Uart_decode(uartData);
	}
}


UartMsg_t GAS_Uart_encode(uint8_t id, uint16_t data)
{
	UartMsg_t Output;
	UartMsgData_t Data;
	Data.Data = data;

	Output.ID.B.idCode = IDCODE;
	Output.ID.B.idData = id;
	Output.data1.B.dataCode1 = DATACODE1;
	Output.data2.B.dataCode2 = DATACODE2;
	Output.data3.B.dataCode3 = DATACODE3;
	Output.data1.B.data = Data.S_Data.data1;
	Output.data2.B.data = Data.S_Data.data2;
	Output.data3.B.data = Data.S_Data.data3;
	return Output;
}


void GAS_Uart_decode(uint8_t inputdata)
{

	if((inputdata >> 5) == IDCODE && ReadCount == 0){
			R_ID= inputdata&0b11111;
			ReadCount++;
	}
	else if((inputdata >> 4) == DATACODE1 && ReadCount == 1){
			R_Data.S_Data.data1 = inputdata&0b1111;
			ReadCount++;
	}
	else if((inputdata >> 6) == DATACODE2 && ReadCount == 2){
			R_Data.S_Data.data2 = inputdata&0b111111;
			ReadCount++;
		}
	else if((inputdata >> 6) == DATACODE3 && ReadCount == 3){

			R_Data.S_Data.data3 = inputdata&0b111111;
			ReadCount = 0;
			RecieveData[R_ID] = R_Data.Data;
		}
	else{
		ErrorCount[0]++;
		ReadCount = 0;
	}
}
