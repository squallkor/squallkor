/*
 * SCU_N_Wash_Mode.c
 *
 *  Created on: 2019. 6. 13.
 *      Author: dhshin
 */

#include "include.h"

uint8 n_standby_mode = 0u;
uint8 wheel_stop_flag = 0u;

static void SCU_Wheel_Stop_Check(void);
static void SCU_N_Mode_Condition_Check(void);
static void SCU_Clu_SbwNmode_req_detetct(void);
static void SCU_N_Wash_Mode_Change(void);

void SCU_N_Wash_Mode_Management(void)
{
	SCU_Wheel_Stop_Check();
	SCU_N_Mode_Condition_Check();
	SCU_Clu_SbwNmode_req_detetct();
	SCU_N_Wash_Mode_Change();
}

static void SCU_Wheel_Stop_Check(void){

	if((Apl_WHL_SPD11_CAN.WHL_SPD_FL == 0U)
	&&(Apl_WHL_SPD11_CAN.WHL_SPD_FR == 0U)
	&&(Apl_WHL_SPD11_CAN.WHL_SPD_RL == 0U)
	&&(Apl_WHL_SPD11_CAN.WHL_SPD_RR == 0U)){
		wheel_stop_flag = 1u;
	}
	else{
		wheel_stop_flag = 0u;
	}
}

static void SCU_N_Mode_Condition_Check(void){

	if((IGN_STAT == ON)
	&& (Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_N)
	&& (Apl_SCU11_CAN.SCU_POS_TAR == SCU_Gear_N)
	/*&& (button_status_change_detect == 1U)*/
	&& (wheel_stop_flag == 1U))
	{
		if(n_standby_mode==2)
		{

		}
		else{
			n_standby_mode = 1u;
		}

	}
	if(Apl_SCU11_CAN.SCU_POS_TAR != SCU_Gear_N)
	{
		n_standby_mode = 0u;

	}
}

static void SCU_Clu_SbwNmode_req_detetct(void){

	if((Apl_CLU22_CAN.CF_Clu_SbwNModeReq == 1U)
		&&(n_standby_mode ==1u))
	{
		n_standby_mode = 2U;
	}
	else{
		/* Nothing */
	}
}

uint8 Ig_On_detect_flag = 0;

static void SCU_N_Wash_Mode_Change(void){



	if(n_standby_mode == 1U){
		Apl_SCU11_CAN.SCU_N_MODE = 1U;
	}
	else if(n_standby_mode == 2U){
		Apl_SCU11_CAN.SCU_N_MODE = 2U;

		if((door_open_detect == 1u) ||(Ig_On_detect_flag==1u))
		{
			n_standby_mode = 0;
		}
	}
	else{
		Apl_SCU11_CAN.SCU_N_MODE = 0U;
		Ig_On_detect_flag = 0u;
	}
}
