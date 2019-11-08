/*
 * SBW_Warn_Management.c
 *
 *  Created on: 2019. 6. 3.
 *      Author: dhshin
 */
#include "include.h"

uint8 scu_warn_msg_1_p_fault = 0u;
uint8 scu_warn_msg_2_not_condition = 0u;
uint8 scu_warn_msg_3_brake_condition = 0u;
uint8 scu_warn_msg_4_speed_condition = 0u;
uint8 scu_warn_msg_5_p_pos_again = 0u;
uint8 scu_warn_msg_7_r_pos_again = 0u;
uint8 scu_warn_msg_9_scu_fault= 0u;
uint8 scu_warn_msg_13_n_pos_again = 0u;
uint8 scu_warn_msg_14_d_pos_again = 0u;

static void SCU_Warn_Msg_1(void);
static void SCU_Warn_Msg_2(void);
static void SCU_Warn_Msg_3(void);
static void SCU_Warn_Msg_4(void);
static void SCU_Warn_Msg_5(void);
static void SCU_Warn_Msg_7(void);
static void SCU_Warn_Msg_9(void);
static void SCU_Warn_Msg_13(void);
static void SCU_Warn_Msg_14(void);
static void SCU_Ok_Status_detect(void);


void SCU_Warn_Management(void){

	/* Priority check */
	SCU_Warn_Msg_14();	/* 9 */
	SCU_Warn_Msg_13();	/* 8 */
	SCU_Warn_Msg_7();	/* 7 */
	SCU_Warn_Msg_5();	/* 6 */

	SCU_Warn_Msg_4();	/* 5 */
	SCU_Warn_Msg_3();	/* 4 */
	SCU_Warn_Msg_2();	/* 3 */

	SCU_Warn_Msg_9();	/* 2 */
	SCU_Warn_Msg_1();	/* 1 */

	SCU_Ok_Status_detect();

}


static void SCU_Ok_Status_detect(void){


	if((scu_warn_msg_1_p_fault == 0u)
	&&(scu_warn_msg_2_not_condition == 0u)
	&&(scu_warn_msg_3_brake_condition == 0u)
	&&(scu_warn_msg_5_p_pos_again == 0u)
	&&(scu_warn_msg_4_speed_condition== 0u)
	&&(scu_warn_msg_7_r_pos_again == 0u)
	&&(scu_warn_msg_9_scu_fault == 0u)
	&&(scu_warn_msg_13_n_pos_again == 0u)
	&&(scu_warn_msg_14_d_pos_again == 0u)){
		Apl_SCU11_CAN.SCU_WarnMsg =  0u;
	}


}

static void SCU_Warn_Msg_1(void)
{
	static uint16 warn_msg_1_cnt = 0u;

	if((Apl_SCU11_CAN.SCU_WarnMsg == 9u)
	&&(SCU_Tar_Position == 0U)
	&&button_status_change_detect == 1u)
	{
		if(warn_msg_1_cnt>5)
		{
			scu_warn_msg_1_p_fault = 1u;
			Apl_SCU11_CAN.SCU_WarnMsg = 1u;
		}
		else{
			scu_warn_msg_1_p_fault = 0u;
			warn_msg_1_cnt++;
		}
	}
	else{
		warn_msg_1_cnt = 0u;
		scu_warn_msg_1_p_fault = 0u;
	}


}

static void SCU_Warn_Msg_2(void)
{
	static uint8 warn_msg_2_cnt = 0u;
	if(scu_warn_msg_2_not_condition == 1u)
	{
		if(warn_msg_2_cnt>10U)
		{
			scu_warn_msg_2_not_condition = 0u;
			warn_msg_2_cnt = 0u;
		}
		else{
			Apl_SCU11_CAN.SCU_WarnMsg = 2u;
			warn_msg_2_cnt++;

		}
	}
}

static void SCU_Warn_Msg_3(void)
{
	static uint8 warn_msg_3_cnt = 0u;
	if(scu_warn_msg_3_brake_condition==1u)
	{
		if(warn_msg_3_cnt>10U)
		{
			scu_warn_msg_3_brake_condition = 0u;
			warn_msg_3_cnt = 0u;
		}
		else{
			Apl_SCU11_CAN.SCU_WarnMsg = 3u;
			warn_msg_3_cnt++;
		}
	}
}
static void SCU_Warn_Msg_4(void)
{
	static uint8 warn_msg_4_cnt = 0u;
		if(scu_warn_msg_4_speed_condition==1u)
		{
			if(warn_msg_4_cnt>10U)
			{
				scu_warn_msg_4_speed_condition = 0u;
				warn_msg_4_cnt = 0u;
			}
			else{
				Apl_SCU11_CAN.SCU_WarnMsg = 4u;
				warn_msg_4_cnt++;
			}
		}


}
static void SCU_Warn_Msg_5(void)
{
	static uint8 warn_msg_5_cnt = 0u;

	if(scu_warn_msg_5_p_pos_again==1u)
	{
		if(warn_msg_5_cnt>10U)
		{
			scu_warn_msg_5_p_pos_again = 0u;
			warn_msg_5_cnt = 0u;
		}
		else
		{
			Apl_SCU11_CAN.SCU_WarnMsg = 5u;
			warn_msg_5_cnt++;
		}
	}
}

static void SCU_Warn_Msg_7(void)
{
	static uint8 warn_msg_7_cnt = 0u;

	if (scu_warn_msg_7_r_pos_again == 1u)
	{
		if (warn_msg_7_cnt > 10U) {

			scu_warn_msg_7_r_pos_again = 0u;
			warn_msg_7_cnt = 0u;
		}
		else
		{
			Apl_SCU11_CAN.SCU_WarnMsg = 7u;
			warn_msg_7_cnt++;
		}
	}

}

static void SCU_Warn_Msg_9(void)
{

	if((SCU_FAULT_State_Flag.EEPROM_Checksum_Error_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag == 0u)			/* YES	6 SEC*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag== 0u)				/* YES  6 SEC*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Power_Open_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Power_Short_flag == 0u)			/* NO*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag == 0u)				/* YES  6 SEC*/
		&&(SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag == 0u)			/* YES  6 SEC*/
		&&(SCU_FAULT_State_Flag.Motor_Battery_Open_flag == 0u)				/* YES  MOTOR BATTERY 1 SEC*/
		&&(SCU_FAULT_State_Flag.Motor_UVW_Short_flag == 0u)					/* YES  UVW SHORT(OVERCURRENT) 50 msec*/
		&&(SCU_FAULT_State_Flag.Motor_Over_Current_flag == 0u)				/* YES  UVW SHORT(OVERCURRENT) 50 msec*/
		&&(SCU_FAULT_State_Flag.Motor_System_error_flag == 0u)				/* YES	Hall ic move , inhibit stop 20 count Not move detect*/
		&&(SCU_FAULT_State_Flag.Motor_Operate_error_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.HallIC_Battery_Short_flag == 0u)			/* YES  Open 1 sec*/
		&&(SCU_FAULT_State_Flag.GateDriver_Error_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.P_Pos_Output_Battery_Short_flag == 0u)		/* NO*/
		&&(SCU_FAULT_State_Flag.P_Pos_Output_Battery_Open_flag == 0u)		/* NO*/
		&&(SCU_FAULT_State_Flag.ACT_Init_Error_flag == 0u)					/* NO*/
		&&(SCU_FAULT_State_Flag.Hall_IC_Open_flag == 0u)					/* YES  bat short  1 sec*/
		&&(SCU_FAULT_State_Flag.Motor_UVW_GND_Short_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.ACT_Stuck_flag == 0u)						/* YES  30 sec*/
		&&(SCU_FAULT_State_Flag.HallIC_Power_Open_flag == 0u)				/* NO*/
		&&(SCU_FAULT_State_Flag.HallIC_Power_Short_flag == 0u)				/* NO*/
		/*&&(Pwr_Vehicle_Over_BAT  == 0U)*/
		/*&&(Pwr_Vehicle_Under_BAT == 0U)*/
		/*&&(Immediate_Under_BAT   == 0U)*/
		/*&&(SCU_FAULT_State_Flag.P_Can_Bus_Off_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.P_CAN_EMS_Timeout_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.P_CAN_TCU_Timeout_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.P_CAN_ESC_Timeout_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.P_CAN_CGW_Timeout_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.L_CAN_TCU_Timeout_flag == 0u)*/
		/*&&(SCU_FAULT_State_Flag.P_CAN_SBW_Timeout_flag == 0u)*/		/*&&(SCU_FAULT_State_Flag.P_CAN_SBW_ERROR_flag == 0u)*/
		){
			scu_warn_msg_9_scu_fault =  0u;
		}
	else{
		scu_warn_msg_9_scu_fault = 1u;
	}

	if (scu_warn_msg_9_scu_fault == 1u) {
		Apl_SCU11_CAN.SCU_WarnMsg = 9u;
	}
	else{
		/*Apl_SCU11_CAN.SCU_WarnMsg = 0u;*/
	}

}

static void SCU_Warn_Msg_13(void)
{
	static uint8 warn_msg_13_cnt = 0u;

	if (scu_warn_msg_13_n_pos_again == 1u) {
		if (warn_msg_13_cnt > 10U)
		{
			scu_warn_msg_13_n_pos_again = 0u;
			warn_msg_13_cnt = 0u;
		}
		else
		{
			Apl_SCU11_CAN.SCU_WarnMsg = 13u;
			warn_msg_13_cnt++;
		}
	}

}

static void SCU_Warn_Msg_14(void)
{
	static uint8 warn_msg_14_cnt = 0u;

	if (scu_warn_msg_14_d_pos_again == 1u)
	{
		if (warn_msg_14_cnt > 10U)
		{

			scu_warn_msg_14_d_pos_again = 0u;
			warn_msg_14_cnt = 0u;
		}
		else
		{
			Apl_SCU11_CAN.SCU_WarnMsg = 14u;
			warn_msg_14_cnt++;
		}
	}

}


