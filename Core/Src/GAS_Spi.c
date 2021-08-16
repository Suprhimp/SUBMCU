/*
 * GAS_SPI.c
 *
 *  Created on: Mar 5, 2021
 *      Author: aswww
 */

#include "GAS_Spi.h"

uint16_t MCP3204[7];

uint16_t GAS_Spi_TransmitReceive(unsigned short data){

	uint8_t buff[3];

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);

	buff[0] = 0x06|((data & 0x07)>>2);
	buff[1] = ((data&0x07)<<6);
	buff[2] = 0;

	HAL_SPI_TransmitReceive(&hspi2,buff,buff,3,5);

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);

	return (uint16_t)((buff[1] & 0x0f)<<8) | buff[2];

}

void GAS_Spi_getAll(){
	for(unsigned short i=0;i<8;i++){
		MCP3204[i] = GAS_Spi_TransmitReceive(i);
	}
}



