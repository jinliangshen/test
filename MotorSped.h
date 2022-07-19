/*
 * PubFun.h
 *
 *  Created on: 2019-3-30
 *      Author: 
 */

#ifndef MOTOR_SPEED_H
#define MOTOR_SPEED_H


/**************************************************************************************************
define
 **************************************************************************************************/
#define TIME_STM0_045MS   			(45000)		// STM0--Frequency-100MHz
#define RT_DIR_CLOCKWISE  			(0)
#define RT_DIR_ANTICLOCKWISE  		(1)

/**************************************************************************************************
typedef  struct
 **************************************************************************************************/

typedef struct MOTORSPEDPAR_STRUCT_DEF{

	uint32 	TimerNow;
	uint32  	TimerBak;
	uint32	TimerDeta;

	sint16     RtPos;
	sint16     RtOrignPos;  
	sint16     RtPosDeta;
	uint16     MotorPoles;
	uint16     RtPoles;
	uint16     MotorPolesVsRtPoles;
	sint16     MotorElecPos;
	sint16     MotorElecPosLast;
	sint16     MotorElecPosDeta;
	uint16     RtDir;

	uint8       RtFault;

	uint16     OnPowerRtWaitTime;
	uint16	OnPowerRtWaitTimeCnt;
	sint16     MotorSped01Hz;
	sint16     MotorSpedOut;

	FST_ORDER_FLT_S16_STRUCT  MotorSpedFlt;
}MOTORSPEDPAR_STRUCT;

extern MOTORSPEDPAR_STRUCT   gMotorSped;
/**************************************************************************************************
extern  function
**************************************************************************************************/
extern  void CalMotorSped(void);
extern  void InitMotorSpedPar(void);


#endif /* MOTOR_SPEED_H */
