/*
 * SCU_EPB_Control.c
 *
 *  Created on: 2019. 9. 16.
 *      Author: dhshin
 */

#include "include.h"

static void Ig_Off_Auto_N_P(void);
static void Door_Open_Auto_P(void);
static void Auto_P_Enter_Safety(void);
static void NOT_AUTO_P_Enter_Safety(void);
static void SlopeWay_Check(void);
static void P_Pos_Speed_Detect(void);
static void EPB_REQ_Function(void);
static void EPB_REQ_NOT_P_Function(void);
static void EPB_Req_Flag_Check(void);
static void Door_Open_Detect(void);
static void Door_Open_Auto_P_Condition(void);

static uint8 p_pos_speed_detect_flag = 0u;						/* P position Speed detect  Flag   */


void SCU_EPB_Control_Main(void)
{
	/* Auto P Enter Logic */
	Ig_Off_Auto_N_P();			 								/* Driving 2Kph Up, Ignition OFF */
	Door_Open_Auto_P();											/* Driving 2Kph down, Door Open  */
	Auto_P_Enter_Safety();										/* Auto P Logic Condition   	 */

	/* Not Auto P Enter Logic */
	P_Pos_Speed_Detect();										/* Speed Check At P Position 	 */
	SlopeWay_Check();											/* Slope Way Check Logic 		 */
	NOT_AUTO_P_Enter_Safety();									/* Non Auto P Enter */

	/* EPB Control Logic */
	EPB_REQ_Function();											/* */
	EPB_REQ_NOT_P_Function();									/* */
	EPB_Req_Flag_Check();										/* */

}


static void P_Pos_Speed_Detect(void)
{
	static uint16 p_pos_speed_detect_cnt = 0;

	if((trans_vehicle_speed_condition == TRANS_NOT_OK)
		&& (Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_P)
		&& (Apl_EPB11_CAN.EPB_STATUS == 1U))
	{
		if(p_pos_speed_detect_cnt>=100u)			/* 1SEC */
		{
			p_pos_speed_detect_flag = 1u;
		}
		else{
			p_pos_speed_detect_flag = 0u;
			p_pos_speed_detect_cnt++;
		}
	}
	else{

		p_pos_speed_detect_cnt = 0U;
	}
}


/*------------Slope way Check ----------*/
static void SlopeWay_Check(void)
{
	static uint8 slope_auto_epb_onetime_flag = 0u;
	static uint16 slope_detect_cnt = 0u;
	/*static uint16 slope_oentime_cnt = 0u;*/

	if((Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_P)
			&& ((Apl_TCU13_CAN.SLOPE_TCU >=40u) ||
					(Apl_TCU13_CAN.SLOPE_TCU <=24u))
			&& (Apl_EPB11_CAN.EPB_STATUS == 1u)
			&& (trans_vehicle_speed_condition== TRANS_OK)
			&& (slope_auto_epb_onetime_flag ==0u))
	{
		if(slope_detect_cnt >= 1u)							/* 10msec Delay */
		{
			slope_way_detect_flag = 1u;
			slope_auto_epb_onetime_flag = 1u;
		}
		else{
			slope_detect_cnt++;
		}

	}
	else{
		/*slope_way_detect_flag = 0u;*/
		slope_detect_cnt = 0u;
	}

	if(slope_auto_epb_onetime_flag == 1u)					/* EPB IS One time User Desire reflect */
	{
		if(Apl_SCU11_CAN.SCU_POS_ACT != 0u)
		{
			slope_way_detect_flag = 0u;
			slope_auto_epb_onetime_flag = 0u;
		}
		else{

		}
	}
}
/*------------SCU Fault state receive shift P pos ----------*/
void SCU_Fault_State_P_Shift_Detect(void)				/* When Detect P button */
{
	if((SCU_Fault_flag == 1u)
			&& (Apl_EPB11_CAN.EPB_STATUS == 1u)
			&& (trans_vehicle_speed_condition== TRANS_OK))
	{
		SCU_Fault_P_EPB_Req_flag = 1u;
	}
	else
	{

	}
}


/*------------ IG off Auto N / IG off Auto P ----------*/
static void Ig_Off_Auto_N_P(void)
{
	if((trans_vehicle_speed_condition == TRANS_NOT_OK))
	{
		if(Ig_Off_detect_flag == 1U)
		{
			SCU_Tar_Position = 6U;
			SCU_ACT_GO_TO_GEAR_N();
		}
		else
		{

		}
	}
	else if((trans_vehicle_speed_condition == TRANS_OK))
	{
		if ((Ig_Off_detect_flag == 1U)
				/*&& (trans_vehicle_speed_condition == TRANS_OK)*/
				&& ((n_standby_mode == 0U)||(n_standby_mode == 1U))
				/*&&((Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_R) ||(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_D)))*/
				&&((Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_R)
						||(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_N)
						||(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_D)))
			{
				Ig_Off_Auto_P_flag = 1U;
				Ig_Off_detect_flag = 0U;

				if(wheel_stop_flag==1U)
				{
					SCU_Tar_Position = 0U;
					SCU_ACT_GO_TO_GEAR_P();
				}
			}
			else{

			}
	}
	else
	{

	}
}

static void Door_Open_Detect(void)
{
	uint8 static pre_task_door_stat = 0u;
	if((Apl_CGW1_CAN.CF_Gway_DrvDrSw == 1U) && (pre_task_door_stat==0U) && (door_open_detect_once ==0U))
	{
		door_open_detect = 1u;
	}
	else if((Apl_CGW1_CAN.CF_Gway_DrvDrSw == 0U) && (pre_task_door_stat ==1U))
	{
		door_open_detect = 0u;
	}
	else
	{

	}
	pre_task_door_stat = Apl_CGW1_CAN.CF_Gway_DrvDrSw;

}

static void Door_Open_Auto_P_Condition(void)
{

	if((trans_vehicle_speed_condition == TRANS_OK)
		&&(door_open_detect == 1)
		&&(Apl_CGW1_CAN.CF_Gway_DrvSeatBeltSw == 0U)
		&&(Apl_EMS12_CAN.PV_AV_CAN == 0U)
		&&((Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_R)
				|| (Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_N)
				||(Apl_SCU11_CAN.SCU_POS_ACT == SCU_Gear_D))
		)
	{
		/* One Time Operation parameter */
		/*door_open_detect_once = 1u;*/
		/*SCU_EPB_Auto_Req_flag = 1u;*/
		Door_Open_Auto_P_flag = 1U;
		door_open_detect = 0U;
	}
	else{

	}

}
/*------------ Door Open Auto P ----------*/
static void Door_Open_Auto_P(void)
{
	Door_Open_Detect();
	Door_Open_Auto_P_Condition();

}


/*------------ Auto P Enter safety ----------*/
static void Auto_P_Enter_Safety(void){

	if((Ig_Off_Auto_P_flag==1)								/* Driving IGN OFF */
		|| (Door_Open_Auto_P_flag ==1U)						/* Low Speed Door Open */
		)
	{
		SCU_EPB_Auto_Req_flag = 1u;
		Ig_Off_Auto_P_flag = 0U;
		Door_Open_Auto_P_flag = 0U;
		p_pos_speed_detect_flag = 0U;
		slope_way_detect_flag = 0u;
	}
	else{

	}
}

/*------------ Non Auto P Enter safety ----------*/
static void NOT_AUTO_P_Enter_Safety(void)
{
	if((SCU_Fault_P_EPB_Req_flag == 1U)						/* SCU Fault Flag 		 */
		|| (slope_way_detect_flag == 1U)					/* Slope detect in P Position */
		|| (p_pos_speed_detect_flag ==1U)					/* P position speed detect    */
			)
	{
		SCU_EPB_Not_Auto_Req_flag = 1U;
		SCU_Fault_P_EPB_Req_flag = 0u;
		slope_way_detect_flag = 0u;
		p_pos_speed_detect_flag = 0u;
	}
	else{

	}
}


static void EPB_REQ_Function(void){

	static uint16 EPB_Req_send_cnt = 0u;

	if((SCU_EPB_Auto_Req_flag==1U)
		&& (detect_target_change_flag == 0U)
		)
	{
		if(EPB_Req_send_cnt <5u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
		}
		else if(EPB_Req_send_cnt <8u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 1u;
		}
		else if(EPB_Req_send_cnt <10u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
		}
		else if(EPB_Req_send_cnt <100u)
		{

		}
		else
		{
			SCU_Tar_Position = 0U;
			SCU_ACT_GO_TO_GEAR_P();
			SCU_EPB_Auto_Req_flag = 0u;
			EPB_Req_send_cnt = 0u;
		}
		EPB_Req_send_cnt++;
	}
	else{
		EPB_Req_send_cnt = 0u;
		SCU_EPB_Auto_Req_flag = 0u;
		Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
	}
}

static void EPB_REQ_NOT_P_Function(void){

	static uint16 EPB_Req_Not_AUTO_P_send_cnt = 0u;

	if((SCU_EPB_Not_Auto_Req_flag==1)
			&& (detect_target_change_flag ==0U)
		)
	{
		if(EPB_Req_Not_AUTO_P_send_cnt <5u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
		}
		else if(EPB_Req_Not_AUTO_P_send_cnt <8u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 1u;
		}
		else if(EPB_Req_Not_AUTO_P_send_cnt <10u)
		{
			Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
		}
		else if(EPB_Req_Not_AUTO_P_send_cnt <100u)
		{

		}
		else
		{
			SCU_EPB_Not_Auto_Req_flag = 0u;
			EPB_Req_Not_AUTO_P_send_cnt = 0u;
		}
		EPB_Req_Not_AUTO_P_send_cnt++;
	}
	else{
		SCU_EPB_Not_Auto_Req_flag = 0u;
		EPB_Req_Not_AUTO_P_send_cnt = 0u;
	}

}

static void EPB_Req_Flag_Check(void)
{
	if((SCU_EPB_Auto_Req_flag == 0u) && (SCU_EPB_Not_Auto_Req_flag==0u))
	{
		Apl_SCU11_CAN.SCU_EPB_REQ = 0u;
	}
}
