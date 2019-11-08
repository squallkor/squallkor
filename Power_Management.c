/********************************************************************************************
| Project Name: Aurix TC2xx
|    File Name: Power_MGT.c
|    Compiler : Tasking Eclipse IDE v6.1.r1
|  Description:
|
|--------------------------------------------------------------------------------------------
|               C O P Y R I G H T
|--------------------------------------------------------------------------------------------
| Copyright (c) by Electronics Architecture Team, all rights reserved
|
|--------------------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|--------------------------------------------------------------------------------------------
| Initials      Name                   Company                             E-mail
| --------      --------------------   ----------------------------    ----------------------
| shum          UmSungHun             SL Corporation                   sunghunum/slworld.com
|                                     Electronics Architecture Team
|
|
|--------------------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|--------------------------------------------------------------------------------------------
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00.00  shum    Creation
|********************************************************************************************/

#include "include.h"


uint8 Ig_Off_detect_flag = 0u;
uint8 IGN_STAT = 0U;
uint8 System_Power = 0U;
uint8 VBAT_STAT = 0U;
uint8 Fault_SBC = 0U;
uint32 V_BAT_SENSE = 0U;
uint32 V_BAT_MOTOR_SENSE = 0U;
uint32 IG1_ADC = 0U;

static void Power_VBat_Check(void);
static void Power_IG1_Check(void);
static void Power_SBC_FLT_Check(void);
static void Power_Status_Check(void);
static void Detect_IG_State(void);
void Power_Management(void){

	Power_VBat_Check();			/* VBAT Voltage Check */
	Power_IG1_Check();			/* IG1 Voltage Check  */
	Power_SBC_FLT_Check();		/* SBC Fault Check	  */
	Power_Status_Check();		/* Statuc Check       */
	Detect_IG_State();
}

/********************************************************************************************
| Project Name: Aurix TC2xx
| File Name: Power_MGT.c
| Func Name: Pwr_Check_IGN_Voltage
| DESCRIPTION: Check Ignition Voltage From Vehicle Power
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00     shum    Creation
| 2018-01-19  1.01     dhshin  Modified
|********************************************************************************************/

static void Power_VBat_Check(void)
{
	static uint8 safe_bat_stat_cnt = 0U;

	V_BAT_SENSE = AI.VIN_System;

	if(V_BAT_SENSE>670U)
	{
		safe_bat_stat_cnt++;
		if(safe_bat_stat_cnt>3U)
		{
			safe_bat_stat_cnt = 3U;
			VBAT_STAT = 1U;
		}
	}
	else{
		safe_bat_stat_cnt = 0U;
		VBAT_STAT = 0U;
	}
}
/********************************************************************************************
| Project Name: Aurix TC2xx
| File Name: Power_MGT.c
| Func Name: Pwr_Check_IGN_Voltage
| DESCRIPTION: Check Ignition Voltage From Vehicle Power
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00     shum    Creation
| 2018-01-19  1.01     dhshin  Modified
|********************************************************************************************/

static void Power_IG1_Check(void) 		/* SW design update complete 08.22 */
{
	static uint16 us_cnt = 0U;

	IG1_ADC = AI.IG1_IN;

	if(IG1_ADC < IGN_Volt_4v )
	{
		IGN_STAT = OFF;
		us_cnt = 0U;
	}
	else if(IG1_ADC > IGN_Volt_65v)	/* DETECT LEVEL 6.5V 19.07.26*/
	{
		us_cnt++;
		if(us_cnt>10U)
		{
			IGN_STAT = ON;
			us_cnt = 100U;
		}
	}
	else
	{
		/* Nothing */
	}
}

void Detect_IG_State(void)
{
	static uint8 pre_task_IG_stat = 0u;
	/*static uint8 ig_change_cnt = 0u;*/

	if((pre_task_IG_stat== 1U) && (IGN_STAT == 0U))
	{
		Ig_Off_detect_flag = 1u;

	}
	else if((pre_task_IG_stat== 0U) && (IGN_STAT == 1U))
	{
		Ig_Off_detect_flag = 0u;
		if(n_standby_mode==2u)
		{
			Ig_On_detect_flag = 1u;
		}
		else{

		}
	}
	else{
	}

	pre_task_IG_stat = IGN_STAT;
}



/********************************************************************************************
| Project Name: Aurix TC2xx
| File Name: Power_MGT.c
| Func Name: Fault_Check_SBC
| DESCRIPTION: Check SBC IC Fault State
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00     shum    Creation
| 2017-11-15  1.01     dhshin  Modified
|********************************************************************************************/

static void Power_SBC_FLT_Check(void)	/* SW design update complete 08.22 */
{
	static uint8 CNT_SBC_Fault = 0U;

	if(Apl_SBC_Flt_Status == ON)		/* SBC Fault is Active Low 17.11.15*/
	{
		Fault_SBC = OFF;
		CNT_SBC_Fault = 0U;
	}
	else if(Apl_SBC_Flt_Status == OFF)
	{
		if(CNT_SBC_Fault < 200U)
		{
			Fault_SBC = OFF;
			CNT_SBC_Fault++;
		}
		else
		{
			Fault_SBC = ON;
		}
	}
	else
	{
		/* Nothing */
	}

}

/********************************************************************************************
| Project Name: Aurix TC2xx
| File Name: Power_MGT.c
| Func Name: Check_Vehicle_Power_State
| DESCRIPTION: Check Vehicle Power State For Use ECU Power State
|
| Date        Ver      Author  Description
| ----------  ---     ------  --------------------------------------------------------------
| 2017-06-16  1.00     shum    Creation
| 2018-08-15  1.01     dhshin  Modified
|********************************************************************************************/

static void Power_Status_Check(void)			/* SW design update complete 08.22 */
{

	/*if((Fault_SBC == OFF)&&(IGN_STAT == OFF))*/
	if(IGN_STAT == OFF)
	{
		System_Power = Vehicle_Power_IGN_OFF;
	}
	/*else if((Fault_SBC == OFF)&&(IGN_STAT == ON))*/
	else if(IGN_STAT == ON)
	{
		System_Power = Vehicle_Power_IGN_ON;
	}
	else{

	}
}
