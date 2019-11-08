/*
 * SCU_SBW_Signal_Detction.c
 *
 *  Created on: 2019. 5. 31.
 *      Author: dhshin
 */
#include "include.h"

#define SCU_DELAY_TIME 1u

uint8 button_status_change_detect = 0u;
uint8 edge_p_button_flag = 0u;
uint8 edge_r_button_flag = 0u;
uint8 edge_n_button_flag = 0u;
uint8 edge_d_button_flag = 0u;

static uint8 P_CNT_Gear_Trans = 0U;
static uint8 R_CNT_Gear_Trans = 0U;
static uint8 N_CNT_Gear_Trans = 0U;
static uint8 D_CNT_Gear_Trans = 0U;


static void Check_P_Position(void);
static void Check_R_Position(void);
static void Check_N_Position(void);
static void Check_D_Position(void);
static void Check_Button_Edge(void);


void SCU_SBW_Signal_Detection(void)
{
	Check_Button_Edge();
	Check_R_Position();
	Check_D_Position();
	Check_N_Position();
	Check_P_Position();
}

static void Check_Button_Edge(void){

	static uint8 pre_task_p_button = 0u;
	static uint8 pre_task_r_button = 0u;
	static uint8 pre_task_n_button = 0u;
	static uint8 pre_task_d_button = 0u;


	if((pre_task_p_button== 2u) && (Apl_LVR12_CAN.CF_Lvr_PButtonStatus== 1u))
	{
		edge_p_button_flag = 1U;
		edge_r_button_flag = 0U;
		edge_n_button_flag = 0U;
		edge_d_button_flag = 0U;

	}
	else if ((pre_task_p_button== 1u) && (Apl_LVR12_CAN.CF_Lvr_PButtonStatus== 2u)){
		edge_p_button_flag = 0u;
		button_status_change_detect = 0U;
	}
	else{

	}

	if((pre_task_r_button== 2u) && (Apl_LVR12_CAN.CF_Lvr_RButtonStatus== 1u))
	{
		edge_p_button_flag = 0U;
		edge_r_button_flag = 1U;
		edge_n_button_flag = 0U;
		edge_d_button_flag = 0U;
	}
	else if((pre_task_r_button== 1u) && (Apl_LVR12_CAN.CF_Lvr_RButtonStatus== 2u)){
		edge_r_button_flag = 0U;
		button_status_change_detect = 0U;
	}
	else{

	}

	if((pre_task_n_button== 2u) && (Apl_LVR12_CAN.CF_Lvr_NButtonStatus== 1u))
	{
		edge_p_button_flag = 0U;
		edge_r_button_flag = 0U;
		edge_n_button_flag = 1U;
		edge_d_button_flag = 0U;
	}
	else if((pre_task_n_button== 1u) && (Apl_LVR12_CAN.CF_Lvr_NButtonStatus== 2u)){
		edge_n_button_flag = 0U;
		button_status_change_detect = 0U;
	}
	else{

	}

	if((pre_task_d_button== 2u) && (Apl_LVR12_CAN.CF_Lvr_DButtonStatus== 1u))
	{
		edge_p_button_flag = 0U;
		edge_r_button_flag = 0U;
		edge_n_button_flag = 0U;
		edge_d_button_flag = 1U;
	}
	else if((pre_task_d_button== 1u) && (Apl_LVR12_CAN.CF_Lvr_DButtonStatus== 2u))
	{
		edge_d_button_flag = 0U;
		button_status_change_detect = 0U;
	}
	else{

	}

	pre_task_p_button = Apl_LVR12_CAN.CF_Lvr_PButtonStatus;
	pre_task_r_button = Apl_LVR12_CAN.CF_Lvr_RButtonStatus;
	pre_task_n_button = Apl_LVR12_CAN.CF_Lvr_NButtonStatus;
	pre_task_d_button = Apl_LVR12_CAN.CF_Lvr_DButtonStatus;

}

/* Button P detection Function*/
static void Check_P_Position(void) {


	if ((edge_p_button_flag == 1)) {
		if (P_CNT_Gear_Trans < SCU_DELAY_TIME) {
			P_CNT_Gear_Trans++;
		} else {
			SCU_Tar_Position = 0U;
			button_status_change_detect = 1u;

			if(pre_task_tar_pos==SCU_Gear_P)
			{
				scu_warn_msg_5_p_pos_again = 1u;
			}
			else{
				SCU_Fault_State_P_Shift_Detect();
			}
			edge_p_button_flag = 0u;
		}
	}
	else{
		P_CNT_Gear_Trans = 0;
	}
}

/* Button R detection Function */
static void Check_R_Position(void) {
	if ((edge_r_button_flag == 1) ) {
		if (R_CNT_Gear_Trans < SCU_DELAY_TIME) {
			R_CNT_Gear_Trans++;
		} else {
			SCU_Tar_Position = SCU_Gear_R;
			button_status_change_detect = 1u;

			if(pre_task_tar_pos==SCU_Gear_R)
			{
				scu_warn_msg_7_r_pos_again = 1u;
			}else{

			}
			edge_r_button_flag = 0U;
		}
	}
	else{
		R_CNT_Gear_Trans = 0u;
	}

}

/* Button N detection Function */
static void Check_N_Position(void) {
	if ((edge_n_button_flag == 1)) {
		if (N_CNT_Gear_Trans < SCU_DELAY_TIME) {
			N_CNT_Gear_Trans++;
		} else {
			SCU_Tar_Position = SCU_Gear_N;
			button_status_change_detect = 1u;

			if(pre_task_tar_pos==SCU_Gear_N)
			{
				scu_warn_msg_13_n_pos_again = 1u;
			}
			else{

			}
			edge_n_button_flag = 0U;
		}
	}
	else{
		N_CNT_Gear_Trans = 0u;
	}
}

/* Button D detection Function */
static void Check_D_Position(void) {
	if ((edge_d_button_flag == 1) ) {
		if (D_CNT_Gear_Trans < SCU_DELAY_TIME) {
			D_CNT_Gear_Trans++;
		} else {
			SCU_Tar_Position = SCU_Gear_D;
			button_status_change_detect = 1u;

			if(pre_task_tar_pos==SCU_Gear_D)
			{
				scu_warn_msg_14_d_pos_again = 1u;
			}
			else{

			}
			edge_d_button_flag = 0u;
		}
	}
	else{
		D_CNT_Gear_Trans = 0;
	}

}


