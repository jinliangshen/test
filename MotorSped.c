/*
 * PubFunc.c
 *
 *  Created on: 2019-3-30
 *      Author: 
 */
#include "Cpu/Std/Platform_Types.h"
#include "Ifx_Types.h"
#include "LD_STM_Exp.h"
#include "AD2S1210_Parall.h"
#include "PubFun.h"
#include "MotorSped.h"
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
MOTORSPEDPAR_STRUCT   gMotorSped;
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
void CalMotorSped(void)
{
	uint16 m_RtPos,m_AbsDetaPos;
	uint32 m_Time,m_Data;
	sint16 m_Sped;
	//  Timer
	gMotorSped.TimerNow = Get_Timer0();
	gMotorSped.TimerDeta = gMotorSped.TimerNow - gMotorSped.TimerBak;
	if(gMotorSped.TimerDeta < TIME_STM0_045MS)	{  return; }
	gMotorSped.TimerBak = gMotorSped.TimerNow;
	//  Pos
	m_RtPos = Read_Position();
	// fault
	gMotorSped.RtFault = Read_Fault();
	if(gMotorSped.RtDir == RT_DIR_CLOCKWISE)
	{
		m_RtPos = 65535 - m_RtPos;
	}
	if(gMotorSped.RtFault == 0)
	{
		gMotorSped.RtPosDeta = (sint16)((sint16)m_RtPos - (sint16)gMotorSped.RtOrignPos);
		gMotorSped.RtOrignPos = (sint16)m_RtPos;
	}
	else
	{
		gMotorSped.RtOrignPos += gMotorSped.RtPosDeta;
	}
	gMotorSped.RtPos = gMotorSped.RtOrignPos;
	gMotorSped.MotorElecPos = (sint16)((sint32)gMotorSped.RtPos * (sint32)gMotorSped.MotorPolesVsRtPoles);
	gMotorSped.MotorElecPosDeta = (sint16)gMotorSped.MotorElecPos - (sint16)gMotorSped.MotorElecPosLast;
	gMotorSped.MotorElecPosLast = gMotorSped.MotorElecPos;
	// speed
	if(gMotorSped.OnPowerRtWaitTimeCnt < gMotorSped.OnPowerRtWaitTime)
	{
		gMotorSped.OnPowerRtWaitTimeCnt++;
		gMotorSped.MotorElecPosDeta  = 0;
	}
	m_AbsDetaPos = ABS(gMotorSped.MotorElecPosDeta);
	/// ---STM--100Mhz
	m_Time = (((uint32)100 * (uint32)1000000) + (gMotorSped.TimerDeta>>1))/gMotorSped.TimerDeta;
	m_Data = (uint32)m_AbsDetaPos * m_Time;
	m_Data = m_Data * 10;	
	m_Sped = (sint16)(m_Data>>16);
	if(gMotorSped.MotorElecPosDeta < 0)
	{
		m_Sped = -m_Sped;
	}
	gMotorSped.MotorSped01Hz = m_Sped;
	gMotorSped.MotorSpedFlt.Input = gMotorSped.MotorSped01Hz;
	FstOrderFltS16(&gMotorSped.MotorSpedFlt,20);
	gMotorSped.MotorSpedOut = gMotorSped.MotorSpedFlt.Out;
}
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
void InitMotorSpedPar(void)
{
	gMotorSped.MotorPoles = 4;
	gMotorSped.RtPoles = 4;
	gMotorSped.MotorPolesVsRtPoles = gMotorSped.MotorPoles / gMotorSped.RtPoles;
	gMotorSped.RtDir = RT_DIR_ANTICLOCKWISE;
	gMotorSped.RtPosDeta = 0;
	gMotorSped.OnPowerRtWaitTimeCnt = 0;
	gMotorSped.OnPowerRtWaitTime = 5;
	gMotorSped.MotorSpedFlt.Input = 0;
	gMotorSped.MotorSpedFlt.InputLast = gMotorSped.MotorSpedFlt.Input;
	gMotorSped.MotorSpedFlt.Out = gMotorSped.MotorSpedFlt.Input;
	gMotorSped.MotorSpedFlt.Flttime = 3;
	
}
/**********************************************************************************
 * Description	*---1210数据更新接口函数
 *********************************************************************************/


	
