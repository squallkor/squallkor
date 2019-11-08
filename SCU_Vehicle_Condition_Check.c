/*
 * SCU_Vehicle_Condition_Check.c
 *
 *  Created on: 2019. 10. 2.
 *      Author: dhshin
 */

#include "include.h"

uint8 HW_Brake_Signal_state = 0u;
uint8 Vehicle_Brake_Signal_state = 0u;

/* Vehicle Brake Total Checking */
static void Vehicle_Brake_state(void);

/* Vehicle HW Brake Checking */
static void Brake_HW_Signal_Detect(void);

/* Vehicle Speed Checking */
static void Check_Vehicle_Speed(void);

/* Vehicle RPM Checking */
static void Check_Engine_RPM(void);

void SCU_Vehicle_Condition_Check_Main(void)
{
	Check_Vehicle_Speed();
	Check_Engine_RPM();
	Brake_HW_Signal_Detect();
	Vehicle_Brake_state();
}


static void Check_Vehicle_Speed(void)
{
	/* Change Condition Wheel FL/FR -> RL/RR 19.10.21 */
	if((Apl_WHL_SPD11_CAN.WHL_SPD_RL < WHL_SPD_P_LIMIT)
	&&(Apl_WHL_SPD11_CAN.WHL_SPD_RR < WHL_SPD_P_LIMIT) )
	{
		trans_vehicle_speed_condition = TRANS_OK;
	}
	else{
		trans_vehicle_speed_condition = TRANS_NOT_OK;
	}

	if((Apl_WHL_SPD11_CAN.WHL_SPD_RL < WHL_SPD_DR_LIMIT)
	&&(Apl_WHL_SPD11_CAN.WHL_SPD_RR < WHL_SPD_DR_LIMIT))
	{
		trans_DR_vehicle_speed_condition = TRANS_OK;
	}
	else{
		trans_DR_vehicle_speed_condition = TRANS_NOT_OK;
	}
}


static void Check_Engine_RPM(void)
{
	/* Change Condition RPM MIN 19.10.21 */
	if((Apl_EMS11_CAN.N < RPM_LIMIT_MAX) && (Apl_EMS11_CAN.N > RPM_LIMIT_MIN))
	{
		trans_vehicle_rpm_condition = TRANS_OK;
	}
	else{
		trans_vehicle_rpm_condition = TRANS_NOT_OK;
	}
}



static void Brake_HW_Signal_Detect(void)
{

	static uint8 brake_hw_cnt = 0u;

	if(AI.Brake_Light_Sw > 1000u)
	{
		if(brake_hw_cnt>10)
		{
			HW_Brake_Signal_state = 1u;
		}
		else{
			brake_hw_cnt++;
		}

	}
	else{
		HW_Brake_Signal_state = 0u;
		brake_hw_cnt = 0u;
	}
}

static void Vehicle_Brake_state(void)
{
	if((Apl_EMS12_CAN.BRAKE_ACT == CAN_BRAKE_ON) || (HW_Brake_Signal_state == 1U))
	{
		Vehicle_Brake_Signal_state = 1u;
	}
	else{
		Vehicle_Brake_Signal_state = 0u;
	}
}

