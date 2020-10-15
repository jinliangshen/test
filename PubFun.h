/*
 * PubFun.h
 *
 *  Created on: 2019-3-30
 *      Author: 
 */

#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H


/**************************************************************************************************
define
 **************************************************************************************************/
#define 		Max(x,y)	    	(((x)>(y))?(x):(y))             //ȡ��ֵ����(����������������)
#define 		Min(x,y)			(((x)<(y))?(x):(y))             //ȡСֵ����(����������������)
#define 		ABS(x)         		(((x)>0)?(x): -(x))
/**************************************************************************************************
typedef  struct
 **************************************************************************************************/
typedef struct FST_ORDER_FLT_S16_STRUCT_DEF{
    	sint16		Input;                  	//	��������ֵ
	sint16     		InputLast;		       //	ǰһ������ֵ
	sint32		OutTotal;               	//	s32��ʽ���˲����������˲�����
	sint16		Out;				    	//	ǰһ�����ֵ
	uint16		Flttime;		        	//	�˲�ʱ�䳣��,��λms
}FST_ORDER_FLT_S16_STRUCT; //һ�׹����˲��õ����ݽṹ������int������

typedef struct FST_ORDER_FLT_S32_STRUCT_DEF{
	sint32     		Input;                  	//��������ֵ
	sint32     		InputLast;              	//ǰһ������ֵ
	sint64     		OutTotal;               	// s64��ʽ���˲����������˲�����
	sint32     		Out;		            		// ǰһ�����ֵ
	uint16 		Flttime;				// �˲�ʱ�䳣��,��λms
}FST_ORDER_FLT_S32_STRUCT; //һ�׹����˲��õ����ݽṹ,����long������

typedef struct FST_ORDER_FLT_F32_STRUCT_DEF{
	float32			Input;                  //	��������ֵ
	float32     		InputLast;           //	ǰһ������ֵ
	float32     		OutTotal;            // 	float32��ʽ���˲����������˲�����
	float32     		Out;		            	// ǰһ�����ֵ
	float32 			Flttime;				// �˲�ʱ�䳣��,��λms
}FST_ORDER_FLT_F32_STRUCT; //һ�׹����˲��õ����ݽṹ,����long������


typedef struct SIGNAL_TABLE_INFO_STRUCT_DEF{
	uint16  		*TableIndex;
	uint16		Size;
	float32		Max;                   
	float32		Min;		           
	float32     	Step;	 
}SIGNAL_TABLE_INFO_STRUCT;


typedef struct DERATE_QUADRANGLE_STRUCT_DEF{
	float32          	Input;
	float32           OutPut;
	float32           DownDeratePoint;
	float32   		DownErrPoint;
	float32           UpDeratePoint;
	float32           UpErrPoint;
	uint16            Flag;
	float32           Point;
}DERATE_QUADRANGLE_STRUCT;


typedef struct AVERAGE_FLT_S32_STRUCT_DEF{

	sint16		Input;
	sint16		InputData[1000];
	sint32            InputTotal;
	uint16             DataIndex;
	uint16             Flag;
	sint16             OutPut;	
}AVERAGE_FLT_S32;

/**************************************************************************************************
extern  function
**************************************************************************************************/
extern void FstOrderFltS16(FST_ORDER_FLT_S16_STRUCT *pFlt , uint16 Fc);
extern void FstOrderFltS32(FST_ORDER_FLT_S32_STRUCT *pFlt , uint16 Fc);
extern float32 GetAdSampleSignalValue(uint16 m_SignalAD,SIGNAL_TABLE_INFO_STRUCT m_TableInfo);
extern void  DerateForQuadrangleDown(DERATE_QUADRANGLE_STRUCT * pDerate, uint16 m_PointOffset);
extern void  DerateForQuadrangleUp(DERATE_QUADRANGLE_STRUCT * pDerate, uint16 m_PointOffset);
extern void  SlidAverageFltS32(AVERAGE_FLT_S32 *pAverageFltS32, uint16 DataLength);
extern uint16 gIgbtTemp[];
extern SIGNAL_TABLE_INFO_STRUCT   gAdSignal[];


#endif /* PUBLIC_FUNCTION_H */
