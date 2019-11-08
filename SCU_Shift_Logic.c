/*
 * SCU_Shift_Logic.c
 *
 *  Created on: 2019. 9. 10.
 *      Author: dhshin
 */

/*
 * D->N	160
 * N->R 160
 * R->P 250
 */

#include "include.h"


/******************** Variable ***********************/
uint8 slope_way_detect_flag = 0u; 						/* Slope Way detect flag    - ?? angle of degrees */
uint8 DP_Check_flag = 0u;
uint8 RP_Check_flag = 0u;
uint8 PR_Check_flag = 0u;
uint8 PN_Check_flag = 0u;
uint8 PD_Check_flag = 0u;

uint8 trans_vehicle_speed_condition = 0U;				/* P TRANS Possible Speed   - 2kph */
uint8 trans_DR_vehicle_speed_condition = 0U;			/* DR TRANS Possible Speed  - 8kph */
uint8 trans_vehicle_rpm_condition = 0u;					/* TRANS Possible RPM 		- 3008 */
uint8 Ig_Off_Auto_P_flag = 0u;							/* IG Off Auto P Flag 		 */
uint8 Ig_Off_Auto_N_flag = 0u;							/* IG Off Auto N Flag 	 	 */
uint8 Door_Open_Auto_P_flag = 0u;						/* Door Open Auto P Flag 	 */
uint8 SCU_EPB_Auto_Req_flag = 0u;						/* SCU Auto P & EPB Req Flag 		 */
uint8 SCU_EPB_Not_Auto_Req_flag = 0u;					/* SCU Not Auto P & EPB Req Flag 		 */
uint8 SCU_Fault_P_EPB_Req_flag = 0u;					/* SCU EPB Req Not Auto P Flag */
uint8 door_open_detect_once = 0u;						/* Door Open detect one time */
uint8 door_open_detect = 0u;
uint8 detect_target_change_flag = 0u;
uint8 p_rnd_detect_flag = 0u;
static void SCU_Trans_Condition_check(void);
static void Trans_P_Condition_Check(void);
static void Trans_P_R_Condition_Check(void);
static void Trans_P_N_Condition_Check(void);
static void Trans_P_D_Condition_Check(void);
/*static void IG_OFF_Trans_P_Condition_Check(void);*/
static void Trans_R_Condition_Check(void);
static void Trans_R_P_Condition_Check(void);
static void Trans_N_Condition_Check(void);
static void IG_OFF_Trans_N_Condition_Check(void);
static void Trans_D_Condition_Check(void);
static void DP_Shift_Control_Logic(void);
static void RP_Shift_Control_Logic(void);
static void PR_Shift_Control_Logic(void);
static void PN_Shift_Control_Logic(void);
static void PD_Shift_Control_Logic(void);
static void Detect_Change_Target(void);
/*static void DP_RP_Check(void);*/

/*------------ SCU_Shift App Main -------------*/
void SCU_Shift_application(void)
{
	SCU_Vehicle_Condition_Check_Main();
	DP_Shift_Control_Logic();
	RP_Shift_Control_Logic();
	PR_Shift_Control_Logic();
	PN_Shift_Control_Logic();
	PD_Shift_Control_Logic();
	if((button_status_change_detect==1U))
	{
		SCU_Trans_Condition_check();
	}
	else
	{

	}
	Detect_Change_Target();
	/*DP_RP_Check();*/

}

static void Detect_Change_Target(void)
{
	if(pre_task_tar_pos == SCU_Tar_Position)
	{
		detect_target_change_flag = 0u;
	}
	else{
		detect_target_change_flag = 1u;
	}
}

static void SCU_Trans_Condition_check(void)
{
	if(IGN_STAT == ON){
		if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_P)
		{
			Trans_P_Condition_Check();
		}
		else if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_R)
		{
			Trans_R_Condition_Check();
		}
		else if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_N)
		{
			Trans_N_Condition_Check();
		}
		else if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_D)
		{
			Trans_D_Condition_Check();
		}
		else{

		}
	}
	else{
		if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_P)
		{
			/*IG_OFF_Trans_P_Condition_Check();*/ /* Delete Condition IG OFF P->N Dont work 19.10.21 */
			SCU_Tar_Position = pre_task_tar_pos;
		}
		else if(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_N)
		{
			IG_OFF_Trans_N_Condition_Check();
		}
		else
		{
			SCU_Tar_Position = pre_task_tar_pos;
		}
	}

}

static void Trans_P_R_Condition_Check(void)
{
	/* Trans Ok */
	if(
	/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/		/*ENGINE : RUN*/
	(Vehicle_Brake_Signal_state == BRAKE_ON)			/*Brake State : ON*/
	&& (trans_vehicle_speed_condition == TRANS_OK)		/*VHspeed : 2Kph*/
	&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
	{
		PR_Check_flag = 1U;
		/*SCU_ACT_GO_TO_GEAR_R();*/
	}
	/* Brake Condition Not Ok */
	else if(
    /*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	/*ENGINE : RUN*/
	(Vehicle_Brake_Signal_state == BRAKE_OFF)			/*Brake State : OFF*/
	&& (trans_vehicle_speed_condition == TRANS_OK)		/*VHspeed : 2Kph*/
	&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
	{
		scu_warn_msg_3_brake_condition =  1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
	/* Shift Condition Not Ok */
	else{
		scu_warn_msg_2_not_condition = 1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
}

static void Trans_P_N_Condition_Check(void){
	/* Trans Ok */
	if(Vehicle_Brake_Signal_state == BRAKE_ON){

		PN_Check_flag = 1U;
		/*SCU_ACT_GO_TO_GEAR_N();*/
	}
	/* Brake Condition Not Ok */
	else if(Vehicle_Brake_Signal_state == BRAKE_OFF){
		scu_warn_msg_3_brake_condition = 1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
	else{

	}
}

static void Trans_P_D_Condition_Check(void)
{
	/* Trans Ok */
	if(
	/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/		/*ENGINE : RUN*/
	(Vehicle_Brake_Signal_state == BRAKE_ON)		/*Brake State : ON*/
	&& (trans_vehicle_speed_condition == TRANS_OK)	/*VHspeed : 2Kph*/
	&& (trans_vehicle_rpm_condition == TRANS_OK))	/*Engine Rpm : 3008 rpm*/
	{
		PD_Check_flag = 1U;
		/*SCU_ACT_GO_TO_GEAR_D();*/
	}
	/* Brake Condition Not Ok */
	else if(
	/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	/*ENGINE : RUN*/
	(Vehicle_Brake_Signal_state == BRAKE_OFF)			/*Brake State : OFF*/
	&& (trans_vehicle_speed_condition == TRANS_OK)		/*VHspeed : 2Kph*/
	&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
	{
		scu_warn_msg_3_brake_condition =  1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
	/* Shift Condition Not Ok */
	else{
		scu_warn_msg_2_not_condition = 1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}

}

static void Trans_P_Condition_Check(void){

	if(SCU_Tar_Position== SCU_Gear_P)
	{
		SCU_ACT_GO_TO_GEAR_P();
	}else if(SCU_Tar_Position== SCU_Gear_R) 	/* IG : ON, SCU_TAR : R*/
	{
		Trans_P_R_Condition_Check();
	}
	else if(SCU_Tar_Position == SCU_Gear_N) 	/* IG : ON, SCU_TAR : N*/
	{
		Trans_P_N_Condition_Check();
	}
	else if (SCU_Tar_Position== SCU_Gear_D)		/* IG : ON*/
	{
		Trans_P_D_Condition_Check();
	}
	else{
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
}

#if 0
static void IG_OFF_Trans_P_Condition_Check(void)
{
	if (SCU_Tar_Position == SCU_Gear_N) /* IG : ON, SCU_TAR : N */
	{
		/* Trans Ok */
		if (Vehicle_Brake_Signal_state == BRAKE_ON) {
			SCU_ACT_GO_TO_GEAR_N();
		}
		/* Brake Condition Not Ok */
		else if (Vehicle_Brake_Signal_state == BRAKE_OFF) {
			scu_warn_msg_3_brake_condition = 1u;
			SCU_Tar_Position = pre_task_tar_pos;
			button_status_change_detect = 0u;
		} else {

		}
	}
	else{

	}
}
#endif

static void Trans_R_P_Condition_Check(void)
{
	/* Trans Ok */
	if((trans_vehicle_speed_condition == TRANS_OK))		/* VHspeed : 2Kph */
	{
		RP_Check_flag = 1U;
	/*	if(RP_flag==1u)
		{
			//RP_Check_flag = 1U;
		}
		else {
			SCU_ACT_GO_TO_GEAR_P();
		}*/

	}
	/* Speed Condition Not Ok */
	else
	{
		scu_warn_msg_4_speed_condition = 1u;
		SCU_Tar_Position = pre_task_tar_pos;
		button_status_change_detect = 0u;
	}
}

static void Trans_R_Condition_Check(void){

		if (SCU_Tar_Position== SCU_Gear_P)
		{
			Trans_R_P_Condition_Check();
		}
		else if (SCU_Tar_Position== SCU_Gear_R){
			/* Re Decide Target */
			SCU_ACT_GO_TO_GEAR_R();
		}
		else if (SCU_Tar_Position== SCU_Gear_N){
			/* Trans Ok */
			SCU_ACT_GO_TO_GEAR_N();
		}
		else if (SCU_Tar_Position== SCU_Gear_D) {
			/* Trans Ok */
			if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/			/*ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_ON)				/*Brake State : ON*/
			&& (trans_DR_vehicle_speed_condition == TRANS_OK))		/*VHspeed : 8Kph*/
			{
				SCU_ACT_GO_TO_GEAR_D();
			}
			/* Brake Condition Not Ok */
			else if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/
			(Vehicle_Brake_Signal_state == BRAKE_OFF)				/*Brake State : OFF*/
			&& (trans_DR_vehicle_speed_condition == TRANS_OK))		/*VHspeed : 8Kph*/
			{
				scu_warn_msg_3_brake_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
			/* Shift Condition Not Ok */
			else{
				scu_warn_msg_2_not_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
		}
		else{

		}
}

static void IG_OFF_Trans_N_Condition_Check(void){

	if (SCU_Tar_Position== SCU_Gear_P)
	{
		/* Trans Ok */
		if(trans_vehicle_speed_condition == TRANS_OK)
		{
			SCU_ACT_GO_TO_GEAR_P();
		}
		/* Speed Condition Not Ok */
		else if(trans_vehicle_speed_condition == TRANS_NOT_OK)
		{
			scu_warn_msg_4_speed_condition = 1u;
			SCU_Tar_Position = pre_task_tar_pos;
			button_status_change_detect = 0u;
		}
		else{
			/*scu_warn_msg_2_not_condition = 1u;
			SCU_Tar_Position = pre_task_tar_pos;
			button_status_change_detect = 0u;*/
		}
	}
	else{

	}
}

static void Trans_N_Condition_Check(void){

		if (SCU_Tar_Position== SCU_Gear_P)
		{
			/* Trans Ok */
			if(trans_vehicle_speed_condition == TRANS_OK)
			{
				SCU_ACT_GO_TO_GEAR_P();
			}
			/* Speed Condition Not Ok */
			else if(trans_vehicle_speed_condition == TRANS_NOT_OK)
			{
				scu_warn_msg_4_speed_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
			else{
				/*scu_warn_msg_2_not_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;*/
			}
		}
		else if (SCU_Tar_Position== SCU_Gear_R)
		{
			/* Trans Ok */
			if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/		/*ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_ON)			/*Brake State : ON*/
			&& (trans_vehicle_speed_condition == TRANS_OK)		/*VHspeed : 2Kph*/
			&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
			{
				SCU_ACT_GO_TO_GEAR_R();
			}
			/* Brake Condition Not Ok */
			else if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	/*ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_OFF)			/*Brake State : ON*/
			&& (trans_vehicle_speed_condition == TRANS_OK)		/*VHspeed : 2Kph*/
			&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
			{
				scu_warn_msg_3_brake_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
			/* Shift Condition Not Ok */
			else{
				if(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == OFF)
				{
				}
				else{
					scu_warn_msg_2_not_condition = 1u;
				}
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
		}
		else if (SCU_Tar_Position== SCU_Gear_D)
		{
			/* Trans Ok */
			if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	 		/*ENGINE : RUN*/
			(((trans_vehicle_speed_condition == TRANS_OK) 	/*VHspeed : 2Kph*/
			&& (trans_vehicle_rpm_condition == TRANS_OK)		/*Engine Rpm : 3008 rpm*/
			&&(Vehicle_Brake_Signal_state == BRAKE_ON))			/*Brake State : ON*/
			||
			((trans_vehicle_speed_condition == TRANS_NOT_OK)  	/*VHspeed : 2Kph*/
					&& ((Vehicle_Brake_Signal_state == BRAKE_OFF)||(Vehicle_Brake_Signal_state == BRAKE_ON)) 			/*Brake State : ON or OFF*/
			&& ((trans_vehicle_rpm_condition == TRANS_OK)||(trans_vehicle_rpm_condition == TRANS_NOT_OK)))))	/*Engine Rpm : 3008 rpm*/
			{
				SCU_ACT_GO_TO_GEAR_D();
			}
			/* Brake Condition Not Ok */
			else if(
			/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	/*ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_OFF)			/*Brake State : ON*/
			&& (trans_vehicle_speed_condition == TRANS_OK) 		/*VHspeed : 2Kph*/
			&& (trans_vehicle_rpm_condition == TRANS_OK))		/*Engine Rpm : 3008 rpm*/
			{
				scu_warn_msg_3_brake_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
			/* Shift Condition Not Ok */
			else{
				if(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == OFF)
				{
					/* Nothing */
				}
				else{
					scu_warn_msg_2_not_condition = 1u;
				}
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;

			}
		}
		else{

		}
}

static void Trans_D_Condition_Check(void){

	if (SCU_Tar_Position== SCU_Gear_P)
		{
			/* Trans Ok */
			if(trans_vehicle_speed_condition == TRANS_OK)		/* VHspeed : 2Kph*/
			{
				DP_Check_flag = 1;
			}
			/* Speed Condition Not Ok */
			else{
				scu_warn_msg_4_speed_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
		}
		else if (SCU_Tar_Position== SCU_Gear_R)
		{
			/* Trans Ok */
			if(/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/		/* ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_ON)			/* Brake State : ON*/
			&& (trans_DR_vehicle_speed_condition == TRANS_OK))	/* VHspeed : 8Kph*/
			{
				SCU_ACT_GO_TO_GEAR_R();
			}
			/* Brake Condition Not Ok */
			else if(/*(Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == ON)*/	/*ENGINE : RUN*/
			(Vehicle_Brake_Signal_state == BRAKE_OFF)			/*Brake State : ON*/
			&& (trans_DR_vehicle_speed_condition == TRANS_OK))	/*VHspeed : 8Kph*/
			{
				scu_warn_msg_3_brake_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
			/* Shift Condition Not Ok */
			else{
				scu_warn_msg_2_not_condition = 1u;
				SCU_Tar_Position = pre_task_tar_pos;
				button_status_change_detect = 0u;
			}
		}
		else if (SCU_Tar_Position== SCU_Gear_N){
			/* Trans Ok */
			SCU_ACT_GO_TO_GEAR_N();
		}
		else if (SCU_Tar_Position== SCU_Gear_D){
			/* Re Decide Target */
			SCU_ACT_GO_TO_GEAR_D();
		}
		else{
			/* Nothing */
		}
}


static void DP_Shift_Control_Logic(void)
{
	static uint16 Shift_DP_cnt = 0u;

	if((DP_Check_flag == 1u) &&( SCU_Tar_Position == 0u) )
	{
		button_status_change_detect = 0u;
		if(Shift_DP_cnt<=42u)
		{

		}
		else if(Shift_DP_cnt<=45u)
		{
			s32_CurrentShaftPostion_Ref = N_POSITION_DUTY;
		}
		else if(Shift_DP_cnt<=60u)
		{

		}
		else if(Shift_DP_cnt<=75u)
		{
				s32_CurrentShaftPostion_Ref = R_POSITION_DUTY;
		}
		else
		{
			s32_CurrentShaftPostion_Ref = P_POSITION_DUTY;
			DP_Check_flag = 0U;
			Shift_DP_cnt = 0u;
		}
		Shift_DP_cnt++;
	}
	else{
		DP_Check_flag = 0u;
		Shift_DP_cnt= 0u;
	}
}

static void RP_Shift_Control_Logic(void)
{
	static uint16 Shift_RP_cnt = 0u;

	if((RP_Check_flag == 1u) && ( SCU_Tar_Position == 0u))
	{
		button_status_change_detect = 0u;
		if(Shift_RP_cnt<=75u)
		{

		}
		else
		{
			s32_CurrentShaftPostion_Ref = P_POSITION_DUTY;
			RP_Check_flag = 0u;
			Shift_RP_cnt = 0u;
		}
		Shift_RP_cnt++;
	}
	else{
		RP_Check_flag = 0u;
		Shift_RP_cnt= 0u;
	}
}

uint8 P_pos_wall_delay = 10u;
float P_pos_wall_duty = 85u;

static void PR_Shift_Control_Logic(void)
{

	if((PR_Check_flag == 1u) && (SCU_Tar_Position == 7u))
	{
		button_status_change_detect = 0u;

		if((s32_CurrentShaftPostion >= P_pos_wall_duty ))
		{
			s32_CurrentShaftPostion_Ref = R_POSITION_DUTY;
			PR_Check_flag = 0u;

		}
		else if((s32_CurrentShaftPostion <P_pos_wall_duty )&& (s32_CurrentShaftPostion >80))
		{
			s32_CurrentShaftPostion_Ref = P_END_POSITION_DUTY;
		}
		else{

		}
	}
	else{
		PR_Check_flag = 0u;

	}
}

static void PN_Shift_Control_Logic(void)
{

	if((PN_Check_flag == 1u) && (SCU_Tar_Position == 6u))
	{
		button_status_change_detect = 0u;
		if((s32_CurrentShaftPostion >= P_pos_wall_duty ))
		{
			s32_CurrentShaftPostion_Ref = N_POSITION_DUTY;
			PN_Check_flag = 0u;

		}
		else if((s32_CurrentShaftPostion <P_pos_wall_duty )&& (s32_CurrentShaftPostion >80))
		{
			s32_CurrentShaftPostion_Ref = P_END_POSITION_DUTY;
		}

		else{

		}
	}
	else{
		PN_Check_flag = 0u;
	}
}

static void PD_Shift_Control_Logic(void)
{

	if((PD_Check_flag == 1u) && (SCU_Tar_Position == 5u))
	{
		button_status_change_detect = 0u;
		 if((s32_CurrentShaftPostion >= P_pos_wall_duty ))
		{
			s32_CurrentShaftPostion_Ref = D_POSITION_DUTY;
			PD_Check_flag = 0u;
		}
		else if((s32_CurrentShaftPostion <P_pos_wall_duty )&& (s32_CurrentShaftPostion >80))
		{
			s32_CurrentShaftPostion_Ref = P_END_POSITION_DUTY;
		}
		else{

		}
	}
	else{
		PD_Check_flag = 0u;
	}
}

#if 0
static void DP_RP_Check(void)
{
	if (AI.SENSOR_TEMP < 400) {
		if ((pre_task_tar_pos == 5u) && (SCU_Tar_Position == 0u)) {
			Position_PI.kp = 35; /*real car 90;112;200;*/
			Position_PI.ki = 10; /*0.8;*/
			Position_PI.kd = 4;  /*20;500;0;*/
		} else if ((pre_task_tar_pos == 7u) && (SCU_Tar_Position == 0u)) {
			Position_PI.kp = 35; /*real car 90; 112;200;*/
			Position_PI.ki = 10; /*0.8;*/
			Position_PI.kd = 4;  /* 20;500;0;*/
		} else if ((pre_task_tar_pos == 0u) && (SCU_Tar_Position == 6u)) {
			Position_PI.kp = 180; /* real car 90; 112; 200;*/
			Position_PI.ki = 15;  /* 0.8;*/
			Position_PI.kd = 4;   /* 20; 500; 0;*/
		}
		else if (pre_task_tar_pos == SCU_Tar_Position) {

		} else {
			Position_PI.kp = 230; /* real car 90; 112; 200;*/
			Position_PI.ki = 20;  /* 0.8;*/
			Position_PI.kd = 4;   /* 20; 500; 0;*/
		}
	} else {
		if ((pre_task_tar_pos == 5u) && (SCU_Tar_Position == 0u)) {
			//Position_PI.kp = 48; /*35; real car 90;112;200;*/
			//Position_PI.ki = 10; /*0.8;*/
			//Position_PI.kd = 4;  /*20;500;0;*/
		} else if ((pre_task_tar_pos == 7u) && (SCU_Tar_Position == 0u)) {
			Position_PI.kp = 33.5; /*30; real car 90;112;200;*/
			Position_PI.ki = 10;   /*0.8;*/
			Position_PI.kd = 4;    /*20;500;0;*/
		} else if (pre_task_tar_pos == SCU_Tar_Position) {

		} else {
			Position_PI.kp = 250; /*real car 90;112;200;*/
			Position_PI.ki = 20;  /*0.8;*/
			Position_PI.kd = 4;   /*20;500;0;*/
		}
	}

}

#endif
