/*
 * PubFunc.c
 *
 *  Created on: 2019-3-30
 *      Author: 
 */
#include "Cpu/Std/Platform_Types.h"
#include "Ifx_Types.h"
#include "PubFun.h"

/**********************************************************************************
 * Description	*---
 *********************************************************************************/
uint16  gIgbtTemp[33] =
{
	66,		90,		115,		156,		205,			//	-40-----20
	254,		328,		410,		500,		606,			//	-15----5
	729,		868,		1016,	1171,	1335,		//	10----30
	1507,	1679,	1851,	2023,	2187,		//	35----55
	2343,	2499,	2638,	2769,	2892,		//	60----80
	3006,	3105,	3203,	3285,	3359,		//	85----105
	3432,	3490,	3547						//   110---120
};
uint16  gPT1000Temp[37] =
{
	2204,	2245,	2294,	2343,	2384,		//	-30-----10
	2433,	2474,	2523,	2564,	2621,		//	-5----15
	2654,	2695,	2744,	2785,	2826,		//	20----40
	2867,	2908,	2949,	2990,	3031,		//	45----65
	3072,	3113,	3154,	3195,	3236,		//	70----90
	3269,	3310,	3351,	3391,	3432,		//	95----115
	3465,	3506,	3539,	3580,	3613,		//	120----140
	3654,	3686								//    145----150	
};
SIGNAL_TABLE_INFO_STRUCT   gAdSignal[2] = 
{
	{(uint16 *)gIgbtTemp			,sizeof(gIgbtTemp)		,120		,-40		,5},
	{(uint16 *)gPT1000Temp		,sizeof(gPT1000Temp)		,150		,-30		,5}
};
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
void FstOrderFltS16(FST_ORDER_FLT_S16_STRUCT *pFlt , uint16 Fc)
{    
	sint32  	m_Coff,m_N32;
	sint16  	m_Out;

	m_N32 = (sint32)pFlt->Flttime * (sint32)Fc + 5;
	m_Coff = (sint32)163840 / m_N32;
    
    if(m_Coff != (sint32)32768)
    {
        m_N32 = m_Coff * ((sint32)pFlt->Input - (sint32)pFlt->Out);
        m_N32 = m_N32 + m_Coff * ((sint32)pFlt->InputLast - (sint32)pFlt->Out);

        pFlt->OutTotal = pFlt->OutTotal + m_N32;
        m_Out = pFlt->OutTotal>>15;
    }
    else
    {
        m_Out = pFlt->Input;
        pFlt->OutTotal = (sint32)pFlt->Input<<15;
    }

	pFlt->Out = m_Out;
    pFlt->InputLast = pFlt->Input;
        
    return;
}
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
void FstOrderFltS32(FST_ORDER_FLT_S32_STRUCT *pFlt , uint16 Fc)
{
    	sint64  m_Coff,m_N64;
	sint32  m_Out,m_N32;

	m_N32 = (sint32)pFlt->Flttime * (sint32)Fc + 5;
	m_Coff = (sint64)((sint32)163840 / m_N32);
	if(m_Coff != (sint32)32768)
	{
		m_N64 = m_Coff * ((sint64)pFlt->Input - (sint64)pFlt->Out);
		m_N64 = m_N64 + m_Coff * ((sint64)pFlt->InputLast - (sint64)pFlt->Out); 
		pFlt->OutTotal = pFlt->OutTotal + m_N64;
		m_Out = (sint32)(pFlt->OutTotal>>15);
	}
	else
	{
		m_Out = pFlt->Input;
		pFlt->OutTotal = (sint64)pFlt->Input<<15;
	}
	pFlt->Out = m_Out;
	pFlt->InputLast = pFlt->Input;

	return;
}
/**********************************************************************************
 * Description	*---
 *********************************************************************************/
void FstOrderFltF32(FST_ORDER_FLT_F32_STRUCT *pFlt , float32 Fc)
{
	float32 m_Data,m_Deta;
	float32 m_Out,m_Coff;
	m_Data = pFlt->Flttime * Fc;
	m_Coff = 5.0*1000.0 / (m_Data + 5.0);
	if(m_Data == 0)
	{
		m_Out = pFlt->Input;
		pFlt->OutTotal = pFlt->Input * 1000.0;
	}
	else
	{
		m_Deta = m_Coff * (pFlt->Input - pFlt->Out);
		m_Deta = m_Deta + m_Coff * (pFlt->InputLast - pFlt->Out);
		pFlt->OutTotal = pFlt->OutTotal + m_Deta;
		m_Out = pFlt->OutTotal / 1000.0;
	}
	pFlt->Out = m_Out;
	pFlt->InputLast = pFlt->Input;

	return;
}
/**********************************************************************************
 * Description	resolution--0.01
 *********************************************************************************/
float32 GetAdSampleSignalValue(uint16 m_SignalAD,SIGNAL_TABLE_INFO_STRUCT m_TableInfo)
{
	uint16  * m_pTable;
	uint16  m_Index_L,m_Index_H,m_Index;
	float32 m_Step;
	float32 m_SignalValue;
	sint16  m_DirFlag;
	uint16  m_MaxCunt;
	sint32  m_DetaAD;
	
	m_pTable = m_TableInfo.TableIndex;	
	m_MaxCunt = 0;
       m_Index_L = 0;
	m_Index_H = m_TableInfo.Size - 1;
	m_Index = m_TableInfo.Size>>1;
	m_Step = m_TableInfo.Step;
	m_DirFlag = 0;

	if(m_pTable[m_Index_L] > m_pTable[m_Index_H])
	{
		if(m_SignalAD >= m_pTable[m_Index_L])			//开始查询温度表
		{	
			m_SignalValue = m_TableInfo.Min * 10.0;			// 0.01  resolution
		}
		else if(m_SignalAD <= m_pTable[m_Index_H])
		{
			m_SignalValue = m_TableInfo.Max * 10.0;
		}
		else
		{
			m_DirFlag = 1;
		} 
	}
	else
	{
		if(m_SignalAD >= m_pTable[m_Index_H])			//开始查询温度表
		{	
			m_SignalValue = m_TableInfo.Max * 10.0;		
		}
		else if(m_SignalAD <= m_pTable[m_Index_L])
		{
			m_SignalValue = m_TableInfo.Min * 10.0;		
		}
		else
		{
			m_DirFlag = -1;	
		} 
	}
	if(m_DirFlag != 0)			//开始查询温度表
	{
		while(m_MaxCunt < m_TableInfo.Size)
		{
			if(m_MaxCunt == 0)
			{
				m_MaxCunt = 1;
			}
			else
			{
				m_MaxCunt = m_MaxCunt << 1;					//避免死循环			
			}

			if(m_SignalAD == m_pTable[m_Index])
			{
				m_SignalValue = (((float32)m_Index * m_Step) + m_TableInfo.Min) * 10.0;
				break;
			}
			else if((m_Index_L+1) == m_Index_H)
			{
				m_DetaAD = (sint32)m_pTable[m_Index_L] - (sint32)m_SignalAD;
				m_SignalValue = (((float32)m_DetaAD * (float32)m_Step) * 10.0)/((sint32)m_pTable[m_Index_L] - (sint32)m_pTable[m_Index_H]);
				m_SignalValue += (((float32)m_Index_L * m_Step + m_TableInfo.Min) * 10.0);
				break;
			}
			if(m_SignalAD > m_pTable[m_Index])
			{
				if(m_DirFlag == 1)
				{
					m_Index_H = m_Index;
				}
				else
				{
					m_Index_L = m_Index;
				}
			}
			else
			{
				if(m_DirFlag == 1)
				{
					m_Index_L = m_Index;
				}
				else
				{
					m_Index_H = m_Index;				
				}
			}
			m_Index = (m_Index_L + m_Index_H)>>1;
		}
	} 

	return m_SignalValue;
}

/**********************************************************************************
 * Description	
 *********************************************************************************/
void  DerateForQuadrangleDown(DERATE_QUADRANGLE_STRUCT * pDerate, uint16 m_PointOffset)
{
	float32 m_InputDeta;
	float32 m_Data;
	float32 m_DetaPoint;

	m_DetaPoint = m_PointOffset;		
	if(pDerate->Flag == 1)
	{
		if(pDerate->Input >= pDerate->Point)
		{
			m_InputDeta = pDerate->DownErrPoint - pDerate->Input;
			m_InputDeta = Max(m_InputDeta,0);
			m_Data = m_InputDeta * 1000 / (pDerate->DownErrPoint - pDerate->DownDeratePoint);
			pDerate->OutPut = Min(m_Data,1000);
			pDerate->Point = pDerate->Input;
		}
		else
		{
			if(pDerate->Input <= (pDerate->Point - m_DetaPoint))
			{
				pDerate->Flag = 2;
				pDerate->Point = pDerate->Point - m_DetaPoint;
			}
		}
	}
	else if(pDerate->Flag == 2)
	{
		if(pDerate->Input <= pDerate->Point)
		{
			m_InputDeta = pDerate->UpErrPoint - pDerate->Input; 
			m_InputDeta = Max(m_InputDeta,0);
			m_Data = m_InputDeta * 1000 / (pDerate->UpErrPoint - pDerate->UpDeratePoint);
			pDerate->OutPut = Min(m_Data,1000);
			pDerate->Point = pDerate->Input;
		}
		else
		{
			if(pDerate->Input > (pDerate->Point + m_DetaPoint))
			{
				pDerate->Flag = 1;
				pDerate->Point = pDerate->Point + m_DetaPoint;
			}
		}
	}	
	return;
}
	
