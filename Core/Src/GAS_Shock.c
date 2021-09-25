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

#define SeraRisingTime 265	//FR
#define SeraMaxWidth 8330

#define SeraRisingTime2 133	//FL\\
#define SeraMaxWidth2 4164

const float PI = 3.1415926;
const float i = 90.7;

ShockAngle angle;

CAN_TxHeaderTypeDef canTxHeader;


static float rollCalc(int RW,int RS, float d);
float GAS_Shock_calculateRoll(float Rd, float Ld, int Location);
static float heaveCalc(int RW, int d);
void GAS_Shock_parse(ShockAngle *angle);
void GAS_Shock_Run(unsigned int Angle1, unsigned int Angle2 ,int isUpdated);

void GAS_Shock_init(int angleR, int angleL){
	angle.initAngleR = angleR;
	angle.initAngleL = angleL;
}


void Front_Encoder(){


    float defAngLeft = (float)angle.initAngleL*PI/1800;
    float defAngRight = (float)angle.initAngleR*PI/1800;
    float changeAngLeft;//좌측 각도 변화량
    float changeAngRigt;//우측 각도변화량
    float strokeChangeLeft;//왼쪽에 의한 압축량
    float strokeChangeRight;//우측에 의한 압축량
    float totalRoll;//합
    float heaveLeft;//왼쪽에 의한 히브쇽 압축량
    float heaveRight;//우측에 의한 히브쇽 압축량
    float totalHeave;

    // 롤 변화량 계산
    //좌측 엔코더 각도 변화량(라디안)
    changeAngLeft = fabs((float)angle.AngleL)*PI/1800 - fabs(defAngLeft);
    //우측 엔코더 각도변화량(라디안)
    changeAngRigt = fabs((float)angle.AngleR)*PI/1800 - fabs(defAngRight);

    float midRes_a = pow(145*145-(52*sin(defAngLeft))*(52*sin(defAngLeft)) , 0.5);
    float midRes_b = pow(145*145-(52*sin(defAngLeft+changeAngLeft))*(52*sin(defAngLeft+changeAngLeft)) , 0.5);
    float midRes_c = pow(145*145-(52*sin(defAngRight+changeAngRigt))*(52*sin(defAngRight+changeAngRigt)) , 0.5);
    float midRes_d = pow(145*145-(52*sin(defAngRight))*(52*sin(defAngRight)) , 0.5);

    strokeChangeLeft = (52*cos(defAngLeft) + fabs(midRes_a) )-(52*cos(defAngLeft+changeAngLeft) + fabs(midRes_b) );

    strokeChangeRight = (52*cos(defAngRight)+ fabs(midRes_a) )-(52*cos(defAngRight+changeAngRigt) + fabs(midRes_c) );

    totalRoll = strokeChangeRight + strokeChangeLeft;

    //히브쇽 변화량 계산
    heaveLeft = -52*cos(defAngLeft)+52*cos(defAngLeft-changeAngLeft);

    heaveRight = -52*cos(defAngRight)+52*cos(defAngRight+changeAngRigt);

    totalHeave = heaveLeft + heaveRight ;
    //printf("%f, %f",strokeChangeLeft, strokeChangeRight);
    angle.Roll = fabs(totalRoll);
    angle.Heave = fabs(totalHeave);
}



void Rear_Encoder(){

    float defAngLeft = (float)angle.initAngleL*PI/1800;
    float defAngRight = (float)angle.initAngleR*PI/1800;
    float changeAngLeft;//좌측 각도 변화량
    float changeAngRigt;//우측 각도변화량
    float strokeChangeLeft;//왼쪽에 의한 압축량
    float strokeChangeRight;//우측에 의한 압축량
    float totalRoll;//합
    float heaveLeft;//왼쪽에 의한 히브쇽 압축량
    float heaveRight;//우측에 의한 히브쇽 압축량
    float totalHeave;

    // 롤 변화량 계산
    //좌측 엔코더 각도 변화량(라디안)
    changeAngLeft = fabs((float)angle.AngleL)*PI/1800 - fabs(defAngLeft);
    //우측 엔코더 각도변화량(라디안)
    changeAngRigt = fabs((float)angle.AngleR)*PI/1800 - fabs(defAngRight);

    float midRes_a = pow(160*160-(71*sin(defAngLeft))*(71*sin(defAngLeft)) , 0.5);
    float midRes_b = pow(160*160-(71*sin(defAngLeft+changeAngLeft))*(71*sin(defAngLeft+changeAngLeft)) , 0.5);
    float midRes_c = pow(160*160-(71*sin(defAngRight+changeAngRigt))*(71*sin(defAngRight+changeAngRigt)) , 0.5);
    float midRes_d = pow(160*160-(71*sin(defAngRight))*(71*sin(defAngRight)) , 0.5);

    strokeChangeLeft = (71*cos(defAngLeft) + fabs(midRes_a) )-(71*cos(defAngLeft+changeAngLeft) + fabs(midRes_b) );

    strokeChangeRight = (71*cos(defAngRight)+ fabs(midRes_a) )-(71*cos(defAngRight+changeAngRigt) + fabs(midRes_c) );

    totalRoll = strokeChangeRight + strokeChangeLeft;

    //히브쇽 변화량 계산
    heaveLeft = -64*cos(defAngLeft)+64*cos(defAngLeft-changeAngLeft);

    heaveRight = -64*cos(defAngRight)+64*cos(defAngRight+changeAngRigt);

    totalHeave = heaveLeft + heaveRight ;
    //printf("%f, %f",strokeChangeLeft, strokeChangeRight);
    angle.Roll = fabs(totalRoll);
    angle.Heave = fabs(totalHeave);
}



void GAS_Shock_Run(uint Angle1, uint Angle2 ,int isUpdated){

	uint32_t TxMailBox;

	angle.AngleR = Angle1;
	if(angle.AngleR>=3600)angle.AngleR=0;

	angle.AngleL = Angle2;
	if(angle.AngleL>=3600)angle.AngleL=0;

	if (LOCATION == Shock_Front){
		Front_Encoder();
	}else{

	}

	stm32_1.B.AngleR = angle.AngleR;
	stm32_1.B.AngleL = angle.AngleL;
	stm32_1.B.Roll = (unsigned int)(angle.Roll*10);
//	stm32_1.B.Heave = (unsigned int)(angle.Heave*10);
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader, &stm32_1.TxData[0], &TxMailBox);

}
