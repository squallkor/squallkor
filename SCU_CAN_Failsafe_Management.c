/*
 * SCU_CAN_Failsafe_Management.c
 *
 *  Created on: 2019. 9. 26.
 *      Author: dhshin
 */

#include "include.h"

/* U000100 P CAN BUS OFF */
static void Fault_detect_P_Can_Bus_Off(void);
/* U010000 P CAN EMS CAN Time Out */
static void Fault_detect_P_CAN_EMS_Timeout(void);
/* U010100 P CAN TCU CAN Time Out */
static void Fault_detect_P_CAN_TCU_Timeout(void);
/* U012100 P CAN ESC CAN Time Out */
static void Fault_detect_P_CAN_ESC_Timeout(void);
/* U014600 P CAN CGW CAN Time Out */
static void Fault_detect_P_CAN_CGW_Timeout(void);
/* U110100 LOCAL CAN TCU CAN Time Out */
static void Fault_detect_L_CAN_TCU_Timeout(void);
/* U110300 P CAN SBW CAN Time Out */
static void Fault_detect_P_CAN_SBW_Timeout(void);
/* U110400 L CAN SBW CAN Time Out */
static void Fault_detect_L_CAN_SBW_Timeout(void);
/* U110382 P CAN SBW CAN CHECKSUM/ALIVECNT ERROR */
static void Fault_detect_P_CAN_SBW_ERROR(void);


void CAN_Failsafe_Management(void)
{
	/* U000100 P CAN BUS OFF */
	Fault_detect_P_Can_Bus_Off();
	/* U010000 P CAN EMS CAN Time Out */
	Fault_detect_P_CAN_EMS_Timeout();
	/* U010100 P CAN TCU CAN Time Out */
	Fault_detect_P_CAN_TCU_Timeout();
	/* U012100 P CAN ESC CAN Time Out */
	Fault_detect_P_CAN_ESC_Timeout();
	/* U014600 P CAN CGW CAN Time Out */
	Fault_detect_P_CAN_CGW_Timeout();
	/* U110100 LOCAL CAN TCU CAN Time Out */
	Fault_detect_L_CAN_TCU_Timeout();
	/* U110300 P CAN SBW CAN Time Out */
	Fault_detect_P_CAN_SBW_Timeout();
	/* U110400 L CAN SBW CAN Time Out */
	Fault_detect_L_CAN_SBW_Timeout();
	/* U110382 P CAN SBW CAN CHECKSUM/ALIVECNT ERROR */
	Fault_detect_P_CAN_SBW_ERROR();

}

/* U000100 P CAN BUS OFF */
void Fault_detect_P_Can_Bus_Off(void){

}

/* U010000 P CAN EMS CAN Time Out */
void Fault_detect_P_CAN_EMS_Timeout(void){

	if((g_Timeout_EMS_H12_0_RxTimeout==1u)
		||(g_Timeout_EMS12_0_RxTimeout==1u)
		||(g_Timeout_EMS11_0_RxTimeout==1u)
		/*||(g_Timeout_WHL_PUL11_0_RxTimeout==1u)*/
		||(g_Timeout_WHL_SPD11_0_RxTimeout==1u)
		)
	{
		SCU_FAULT_State_Flag.P_CAN_EMS_Timeout_flag = 1u;
		fgDTC_flag_Lost_Communication_With_EMS = 1;
	}
	else if(g_Timeout_EMS_H12_0_RxTimeout==0u)
	{
		SCU_FAULT_State_Flag.P_CAN_EMS_Timeout_flag = 0u;
		fgDTC_flag_Lost_Communication_With_EMS = 0;
	}

}

/* U010100 P CAN TCU CAN Time Out */
void Fault_detect_P_CAN_TCU_Timeout(void){

	if(g_Timeout_TCU11_0_RxTimeout==1u)
	{
		SCU_FAULT_State_Flag.P_CAN_TCU_Timeout_flag= 1u;
		fgDTC_flag_Lost_Communication_With_TCU = 1u;
	}
	else if(g_Timeout_TCU11_0_RxTimeout==0u)
	{
		SCU_FAULT_State_Flag.P_CAN_TCU_Timeout_flag = 0u;
		fgDTC_flag_Lost_Communication_With_TCU = 0u;
	}
	else{

	}

}

/* U012100 P CAN ESC CAN Time Out */
void Fault_detect_P_CAN_ESC_Timeout(void){

	if((g_Timeout_ESP11_0_RxTimeout==1u)
	||(g_Timeout_EPB11_0_RxTimeout==1u)
	||(g_Timeout_EPB11_0_RxTimeout==1u)
	||(g_Timeout_ESP12_0_RxTimeout==1u))
	{
		SCU_FAULT_State_Flag.P_CAN_ESC_Timeout_flag= 1u;
		fgDTC_flag_Lost_Communication_With_ESC = 1U;
	}
	else if(g_Timeout_ESP11_0_RxTimeout==0)
	{
		SCU_FAULT_State_Flag.P_CAN_ESC_Timeout_flag = 0u;

		fgDTC_flag_Lost_Communication_With_ESC = 0U;
	}
	else{

	}

}

/* U014600 P CAN CGW CAN Time Out */
void Fault_detect_P_CAN_CGW_Timeout(void){

	if((g_Timeout_CGW1_0_RxTimeout==1)
	||(g_Timeout_CGW2_0_RxTimeout==1))
	{
		SCU_FAULT_State_Flag.P_CAN_CGW_Timeout_flag = 1u;
		fgDTC_flag_Lost_Communication_With_GateWay = 1;
	}
	else if(g_Timeout_CGW1_0_RxTimeout==0)
	{
		SCU_FAULT_State_Flag.P_CAN_CGW_Timeout_flag = 0u;
		fgDTC_flag_Lost_Communication_With_GateWay = 0;
	}
	else{

	}
}

/* U110100 LOCAL CAN TCU CAN Time Out */
void Fault_detect_L_CAN_TCU_Timeout(void){

	if(g_Timeout_TCU11_1_RxTimeout ==1)
	{
		SCU_FAULT_State_Flag.L_CAN_TCU_Timeout_flag = 1u;
		fgDTC_flag_Lost_Communication_With_Local_TCU = 1;
	}
	else if(g_Timeout_TCU11_1_RxTimeout ==0)
	{
		SCU_FAULT_State_Flag.L_CAN_TCU_Timeout_flag = 0u;
		fgDTC_flag_Lost_Communication_With_Local_TCU = 0;

	}
	else{

	}

}

/* U110300 P CAN SBW CAN Time Out */
static void Fault_detect_P_CAN_SBW_Timeout(void){

	if(g_Timeout_Lvr12_0_RxTimeout==1U)
	{
		SCU_FAULT_State_Flag.P_CAN_SBW_Timeout_flag= 1u;
		fgDTC_flag_Lost_Communication_With_SBW = 1u;
	}
		else if(g_Timeout_Lvr12_0_RxTimeout==0U)
	{
		SCU_FAULT_State_Flag.P_CAN_SBW_Timeout_flag = 0u;
		fgDTC_flag_Lost_Communication_With_SBW = 0U;
	}
	else{
	}
}
/* U110300 P CAN SBW CAN Time Out */
static void Fault_detect_L_CAN_SBW_Timeout(void){

	if(g_Timeout_Lvr12_1_RxTimeout==1)
	{
		SCU_FAULT_State_Flag.L_CAN_SBW_Timeout_flag= 1u;
		fgDTC_flag_CAN2_Lost_Communication_With_SBW = 1U;
	}
		else if(g_Timeout_Lvr12_1_RxTimeout==0)
	{
		SCU_FAULT_State_Flag.L_CAN_SBW_Timeout_flag = 0u;
		fgDTC_flag_CAN2_Lost_Communication_With_SBW = 0u;
	}
	else{
	}
}
/* U110382 P CAN SBW CAN CHECKSUM/ALIVECNT ERROR */
void Fault_detect_P_CAN_SBW_ERROR(void){

	static uint8 crc_error_cnt = 0u;

	if((lvr_crc_error_flag==1u)||(lvr_ac_error_flag==1u)){
		if(crc_error_cnt>200u)
		{
			/* Error Flag Set */
			SCU_FAULT_State_Flag.P_CAN_SBW_ERROR_flag = 1U;
			fgDTC_flag_TGS_Checksum_Alive_Error = 1u;
		}
		else{
			/* Error Flag Clear */
			SCU_FAULT_State_Flag.P_CAN_SBW_ERROR_flag = 0U;
			crc_error_cnt++;
			fgDTC_flag_TGS_Checksum_Alive_Error = 0u;
		}

	}
	else{
		crc_error_cnt  = 0u;
		fgDTC_flag_TGS_Checksum_Alive_Error = 0u;
	}

}


