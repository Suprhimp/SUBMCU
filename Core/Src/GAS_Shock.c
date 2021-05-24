/*
 * GAS_Shock.c
 *
 *  Created on: 2021. 5. 16.
 *      Author: Suprhimp
 */
#include "GAS_Shock.h"
#include "math.h"

#define Shock_Front 0
#define Shock_Rear 1

#define LOCATION Shock_Front

#define RockerWing_F 52
#define RockerShaft_F 145

#define RockerWing_R 71
#define RockerWingH_R 64
#define RockerShaft_R 160

#define SeraRisingTime 265
#define SeraMaxWidth 8330

#define SeraRisingTime2 133
#define SeraMaxWidth2 4164

const double PI = 3.1415926;
const float i = 90.7;

ShockAngle angle;

CAN_TxHeaderTypeDef canTxHeader;


static float rollCalc(int RW,int RS, float d);
float GAS_Shock_calculateRoll(float Rd, float Ld, int Location);
static float heaveCalc(int RW, int d);
void GAS_Shock_parse(ShockAngle *angle);
void GAS_Shock_Run(pwmIn_t *pwmIn1, pwmIn_t *pwmIn2 ,int isUpdated);

void GAS_Shock_init(int angleR, int angleL){
	angle.initAngleR = angleR;
	angle.initAngleL = angleL;
}

void GAS_Shock_parse(ShockAngle *angle){
	int initAngle=907;
	angle->parsedAngleR = (initAngle + angle->AngleR - angle->initAngleR)*PI/3600;
	angle->parsedAngleL = (initAngle + angle->AngleL - angle->initAngleL)*PI/3600;
}

float GAS_Shock_calculateRoll(float Rd, float Ld, int Location){
	int d1=0,d2=0;
	if (Location == Shock_Front){
		d1 = rollCalc(RockerWing_F,RockerShaft_F,Rd);
		d2 = rollCalc(RockerWing_F,RockerShaft_F,Ld);
	}
	if (Location == Shock_Rear){
		d1 = rollCalc(RockerWing_R,RockerShaft_R,Rd);
		d2 = rollCalc(RockerWing_R,RockerShaft_R,Ld);
	}
	return d1+d2;
}

static float rollCalc(int RW,int RS, float d){

	return ((RW*cos(i)+sqrt(pow(RS,2)-pow(RW*sin(i),2)))-(RW*cos(i+d)+sqrt(pow(RS,2)-pow(RW*sin(i+d),2))));
}

int GAS_Shock_calculateHeave(float Rd, float Ld, int Location){
	int d1,d2;
	if (Location == Shock_Front){
		d1 = heaveCalc(RockerWing_F,Rd);
		d2 = heaveCalc(RockerWing_F,Ld);
	}
	if (Location == Shock_Rear){
		d1 = heaveCalc(RockerWingH_R,Rd);
		d2 = heaveCalc(RockerWingH_R,Ld);
	}
	return d1+d2;
}

static float heaveCalc(int RW, int d){

	return (52*cos(i-d)-RW*cos(i));
}

void GAS_Shock_Run(pwmIn_t *pwmIn1, pwmIn_t *pwmIn2 ,int isUpdated){

	uint32_t TxMailBox;

	angle.AngleR = (3600*(pwmIn1->Width-SeraRisingTime)/(SeraMaxWidth-SeraRisingTime));
	if(angle.AngleR>=3600)angle.AngleR=0;

	angle.AngleL = (3600*(pwmIn2->Width-SeraRisingTime)/(SeraMaxWidth-SeraRisingTime));
	if(angle.AngleL>=3600)angle.AngleL=0;

	GAS_Shock_parse(&angle);

	stm32_1.B.isUpdated = isUpdated;
	stm32_1.B.Sensor0 = GAS_Shock_calculateRoll(angle.parsedAngleR,angle.parsedAngleL,LOCATION);
	stm32_1.B.Sensor1 = GAS_Shock_calculateHeave(angle.parsedAngleR,angle.parsedAngleL,LOCATION);
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader, &stm32_1.TxData[0], &TxMailBox);

}
