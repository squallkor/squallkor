/*************************************************************************************************
| Project Name:	SCU Actuator
|    File Name: SCU_Indicator.c
|    Compiler : Tasking Compiler
|  Description: SCU Actual Position Indicate Application
|
|-------------------------------------------------------------------------------------------------
|               C O P Y R I G H T
|-------------------------------------------------------------------------------------------------
| Copyright (c) by Chassis Electronic Team, all rights reserved
|
|-------------------------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-------------------------------------------------------------------------------------------------
| Initials      Name                   Company                             E-mail
| --------      --------------------   ---------------------------         -----------------------
| SDH           Dong Hyup Shin         SL Corporation        			   dhshin@slworld.com
|
|
|-------------------------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-------------------------------------------------------------------------------------------------
| Date        Ver    Num    Author  Description
| ----------  ---    ---    ------  --------------------------------------------------------------
| 2019-05-31  1.0           SDH     Initiate
**************************************************************************************************/


#include "include.h"

#define DECISION_TIME 1 /* 30ms*/
#define P_POSITION 1000U
#define NOT_P_POSITION 4000U
static uint8 p_position_confirm_cnt = 0u;
static uint8 r_position_confirm_cnt = 0u;
static uint8 n_position_confirm_cnt = 0u;
static uint8 d_position_confirm_cnt = 0u;
static uint8 inter_position_confirm_cnt = 0u;
static uint8 scu_pos_init_flag = 0u;

uint8 SCU_Tar_Position = 0U;
uint8 SCU_Gear_Position = 0U;
uint8 pre_task_tar_pos = 0u;

static void Inhibit_P_Position_Check(void);
static void Inhibit_R_Position_Check(void);
static void Inhibit_N_Position_Check(void);
static void Inhibit_D_Position_Check(void);
static void Inhibit_Inter_Position_Check(void);
static void Output_SCU_SBW_Position(void);
static void Scu_Act_Position_Decision(void);
static void Initialize_Pos_Check(void);
static void SCU_P_Position_PWM_Output(void);
static void Check_Init_Position(void);	/* 19.8.26 Add */
static uint16 P_Position_Pwm = 1000u;

void SCU_Indicator_application(void)
{
	Scu_Act_Position_Decision();
	Output_SCU_SBW_Position();
	SCU_P_Position_PWM_Output();
}

static void SCU_P_Position_PWM_Output(void){


	if(Apl_SCU11_CAN.SCU_POS_ACT == 0U)
	{
		P_Position_Pwm = 1000u;	/* P */
	}
	else{
		P_Position_Pwm = 4000u;  /* Not P*/
	}
	Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_P_POSITION_OUTPUT,P_Position_Pwm);	   /*TOM0_CH11 : P02.3, 	TP141 */


}

static uint8 Array_SCU_tar[4]={0x9u,0x9u,0x9u,0x9u};
static void Output_SCU_SBW_Position(void)
{
	Apl_SCU11_CAN.SCU_POS_ACT = SCU_Gear_Position;
	Apl_SCU11_CAN.SCU_POS_TAR = Array_SCU_tar[3];
	/*Apl_SCU11_CAN.SCU_POS_TAR = SCU_Tar_Position;*/

	pre_task_tar_pos = SCU_Tar_Position;

	Array_SCU_tar[3]= Array_SCU_tar[2];
	Array_SCU_tar[2]= Array_SCU_tar[1];
	Array_SCU_tar[1]= Array_SCU_tar[0];
	Array_SCU_tar[0]= SCU_Tar_Position;

}

static void Inhibit_P_Position_Check(void){
	if(p_position_confirm_cnt>DECISION_TIME){
		SCU_Gear_Position = 0U;
	}
	else{
		p_position_confirm_cnt++;
	}
	r_position_confirm_cnt = 0U;
	n_position_confirm_cnt = 0U;
	d_position_confirm_cnt = 0U;
	inter_position_confirm_cnt = 0U;
}



static void Inhibit_R_Position_Check(void){
	if(r_position_confirm_cnt >DECISION_TIME)
	{
		SCU_Gear_Position = 7U; 	
	}
	else{
		r_position_confirm_cnt++;
	}
	p_position_confirm_cnt = 0U;
	n_position_confirm_cnt = 0U;
	d_position_confirm_cnt = 0U;
	inter_position_confirm_cnt = 0U;

}


static void Inhibit_N_Position_Check(void){
	if(n_position_confirm_cnt >DECISION_TIME)
	{
		SCU_Gear_Position = 6U; 	
	}
	else{
		n_position_confirm_cnt++;
	}
	p_position_confirm_cnt = 0U;
	r_position_confirm_cnt = 0U;
	d_position_confirm_cnt = 0U;
	inter_position_confirm_cnt = 0U;

}

static void Inhibit_D_Position_Check(void){

	if(d_position_confirm_cnt >DECISION_TIME)
	{
		SCU_Gear_Position = 5U; 	
	}
	else{
		d_position_confirm_cnt++;
	}
	p_position_confirm_cnt = 0U;
	r_position_confirm_cnt = 0U;
	n_position_confirm_cnt = 0U;
	inter_position_confirm_cnt = 0U;


}

static void Inhibit_Inter_Position_Check(void){

/*	p_position_confirm_cnt = 0U;
	r_position_confirm_cnt = 0U;
	n_position_confirm_cnt = 0U;

	d_position_confirm_cnt = 0U;*/
	if(inter_position_confirm_cnt >DECISION_TIME)
	{
		SCU_Gear_Position = 14U;
		/*p_position_confirm_cnt = 0U;
		r_position_confirm_cnt = 0U;
		n_position_confirm_cnt = 0U;
		d_position_confirm_cnt = 0U;*/
	}
	else{
		inter_position_confirm_cnt++;
	}
	p_position_confirm_cnt = 0U;
	r_position_confirm_cnt = 0U;
	n_position_confirm_cnt = 0U;
	d_position_confirm_cnt = 0U;

}

static void Scu_Act_Position_Decision(void){

	Initialize_Pos_Check();
	if(scu_pos_init_flag==1)
	{
		if((s32_CurrentShaftPostion >= (69.3))
				&& (s32_CurrentShaftPostion <=(90))){	/* P Position */	/* 81.7*/
			Inhibit_P_Position_Check();
		}
		else if((s32_CurrentShaftPostion >=(43))
				&& (s32_CurrentShaftPostion <= (62.4))) /* R Position */	/* 52.5*/
		{
			Inhibit_R_Position_Check();
		}
		else if((s32_CurrentShaftPostion >=(25.7))
				&& (s32_CurrentShaftPostion <=(40.9))) /* N Position */		/*35.4*/
		{
			Inhibit_N_Position_Check();
		}
		else if((s32_CurrentShaftPostion >=(10))
				&& (s32_CurrentShaftPostion <=(23.5))) /* D Position */		/*18.3*/
		{
			Inhibit_D_Position_Check();
		}
		else{
			Inhibit_Inter_Position_Check();
		}
	}


}

static void Initialize_Pos_Check(void) {

	if (scu_pos_init_flag == 0u) {
		if(INIT_POS_flag==1)
		{
			Check_Init_Position();
			scu_pos_init_flag = 1u;
		}
		else{
			SCU_Gear_Position = 0x9u;			/* Do not Display Initial Position */
			SCU_Tar_Position = 0x9u;			/* Do not Display Initial Position */
		}
	}

}


/* Ver 0.02 Update for HW RESET 19.09.04 */
static void Check_Init_Position(void)	/* 19.8.26 Add */
{

	if ((s32_CurrentShaftPostion >= (69.3))
			&& (s32_CurrentShaftPostion <= (90))) { /* P Position 81.7*/
		SCU_ACT_GO_TO_GEAR_P();
		SCU_Tar_Position = 0x0u;
	} else if ((s32_CurrentShaftPostion >= (43))
			&& (s32_CurrentShaftPostion <= (62.4))) /* R Position 52.5 */
			{
		SCU_ACT_GO_TO_GEAR_R();
		SCU_Tar_Position = 0x7u;
	} else if ((s32_CurrentShaftPostion >= (25.7))
			&& (s32_CurrentShaftPostion <= (40.9))) /* N Position 35.4 */
			{
		SCU_ACT_GO_TO_GEAR_N();
		SCU_Tar_Position = 0x6u;
	} else if ((s32_CurrentShaftPostion >= (10))
			&& (s32_CurrentShaftPostion <= (23.5))) /* D Position 18.3 */
			{
		SCU_ACT_GO_TO_GEAR_D();
		SCU_Tar_Position = 0x5u;
	} else {

	}
}
