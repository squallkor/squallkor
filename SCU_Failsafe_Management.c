/*
 * SCU_FAILSAFE_Management.c
 *
 *  Created on: 2019. 6. 10.
 *      Author: dhshin
 */

#include "include.h"

SCU_FAULT_LIST SCU_FAULT_State_Flag;
uint8 SCU_Fail_Safety = 0u;
uint8 SCU_CAN_Fail_Safety = 0u;

uint8 Immediate_Under_BAT = 0U;
uint8 Pwr_Vehicle_Under_BAT = 0U;
uint8 Pwr_Vehicle_Over_BAT = 0U;
uint8 Indicator_Fault_Check = 0U;
uint8 SCU_Fault_flag = 0U;
uint8 CAN_Fault_flag = 0u;
uint8 motor_stuck_flag = 0u;

uint8 hall_ic_pattern_error_cnt = 0u;
uint8 hall_ic_pattern_recovery_cnt = 0u;
uint8 hall_ic_pattern_error_flag = 0u;

static float pre_s32_CurrentShaftPostion_Ref = 0;
static float pre_s32_CurrentShaftPostion = 0;
static float pre_f32_HallIC_Position = 0;
static float delta_Inhibit_Postion = 0;
static float delta_HallIC_Position = 0;

uint8 detect_Motor_Move_flag = 0u;
uint8 detect_Hall_IC_Move_flag = 0u;
uint8 detect_InhibitSw_Move_flag = 0u;


static void Check_Vehicle_Battery_Over_Voltage(void);
static void Check_Vehicle_Battery_Under_Voltage(void);
static void Check_Vehicle_Battery_immediat_Under_Voltage(void);
static void Shifting_Detect(void);
static void Motor_Stop_Control(void);
static void SCU_Warning_Fault(void);

/* P060141 EEPROM Fault Checksum */
static void Fault_detect_EEPROM(void);
/* P070512 Position Sensor Signal Short */
static void Fault_detect_Pos_Sens_Signal_Short(void);
/* P070514 Position Sensor Signal Open */
static void Fault_detect_Pos_Sens_Signal_Open(void);
/* P070516 Position Sensor Power Open */
static void Fault_detect_Pos_Sens_Power_Open(void);
/* P070517 Position Sensor Power Short */
static void Fault_detect_Pos_Sens_Power_Short(void);
/* P070661 Position Sensor Data Error */
static void Fault_detect_Pos_Sens_Data_Error(void);
/* P070696 Position Sensor Output Error */
static void Fault_detect_Pos_Sens_Output_Error(void);
/* P106114 Motor Battery Open */
static void Fault_detect_Motor_Battery_Open(void);
/* P106012 Motor UVW Battery Open */
static void Fault_detect_Motor_UVW_Short(void);
/* P106400 Motor Motor Over Current */
static void Fault_detect_Motor_Over_Current(void);
/* P106800 Motor System Error */
static void Fault_detect_Motor_System_error(void);
/* P106900 Motor Operate Error */
static void Fauilt_detect_Motor_Operate_error(void);
/* P106A12 HallIC_Battery_Short */
static void Fault_detect_HallIC_Battery_Short(void);
/* P106B00 Gate Driver Error */
static void Fault_detect_GateDriver_Error(void);
/* P106C12 P Position Output Battery Short */
static void Fault_detect_P_Pos_Output_Battery_Short(void);
/* P106C14 P Position Output Battery Open */
static void Fault_detect_P_Pos_Output_Battery_Open(void);
/* P106D00 Actuator Initial Drive Error */
static void Fault_detect_ACT_Init_Error(void);
/* P106D13 Actuator Hall IC Open */
static void Fault_detect_Hall_IC_Open(void);
/* P106D14 Motor UVW Open */
static void Fault_detect_Motor_UVW_GND_Short(void);
/* P106D71 ACT  Stuck */
static void Fault_detect_ACT_Stuck(void);
/* P106E16 HallIC Power Open */
static void Fault_detect_HallIC_Power_Open(void);
/* P106E17 HallIC Power Short */
static void Fault_detect_HallIC_Power_Short(void);

static void CAN_Fault_Reaction(void);

void SCU_Failsafe_Management(void)
{
	if(IGN_STAT == ON)
	{
		if((Pwr_Vehicle_Over_BAT == 1) || (Pwr_Vehicle_Under_BAT==1) || (Immediate_Under_BAT==1))
		{

			if ((s32_CurrentShaftPostion >= (69.3))
					&& (s32_CurrentShaftPostion <= (90))) { /* P Position */	/* 81.7*/
				SCU_ACT_GO_TO_GEAR_P();
			} else if ((s32_CurrentShaftPostion >= (43))
					&& (s32_CurrentShaftPostion <= (62.4))) /* R Position */	/* 52.5*/
					{
				SCU_ACT_GO_TO_GEAR_R();
			} else if ((s32_CurrentShaftPostion >= (25.7))
					&& (s32_CurrentShaftPostion <= (40.9))) /* N Position */	/*35.4*/
					{
				SCU_ACT_GO_TO_GEAR_N();
			} else if ((s32_CurrentShaftPostion >= (10))
					&& (s32_CurrentShaftPostion <= (23.5))) /* D Position */	/*18.3*/
					{
				SCU_ACT_GO_TO_GEAR_D();
			} else {
				SCU_ACT_GO_TO_GEAR_N();
				SCU_EPB_Not_Auto_Req_flag = 1u;
			}
		}
		else if((Pwr_Vehicle_Over_BAT == 0) && (Pwr_Vehicle_Under_BAT == 0) && (Immediate_Under_BAT==0))
		{

			Shifting_Detect();
			/* P060141 EEPROM Fault Checksum */
			Fault_detect_EEPROM();
			/* P070512 Position Sensor Signal Short */
			Fault_detect_Pos_Sens_Signal_Short();
			/* P070514 Position Sensor Signal Open */
			Fault_detect_Pos_Sens_Signal_Open();
			/* P070516 Position Sensor Power Open */
			Fault_detect_Pos_Sens_Power_Open();
			/* P070517 Position Sensor Power Short */
			Fault_detect_Pos_Sens_Power_Short();
			/* P070661 Position Sensor Data Error */
			Fault_detect_Pos_Sens_Data_Error();
			/* P070696 Position Sensor Output Error */
			Fault_detect_Pos_Sens_Output_Error();
			/* P106114 Motor Battery Open */
			Fault_detect_Motor_Battery_Open();
			/* P106012 Motor UVW Battery Open */
			Fault_detect_Motor_UVW_Short();
			/* P106400 Motor Motor Over Current */
			Fault_detect_Motor_Over_Current();
			/* P106800 Motor System Error */
			Fault_detect_Motor_System_error();
			/* P106900 Motor Operate Error */
			Fauilt_detect_Motor_Operate_error();
			/* P106A12 HallIC_Battery_Short */
			Fault_detect_HallIC_Battery_Short();
			/* P106B00 Gate Driver Error */
			Fault_detect_GateDriver_Error();
			/* P106C12 P Position Output Battery Short */
			Fault_detect_P_Pos_Output_Battery_Short();
			/* P106C14 P Position Output Battery Open */
			Fault_detect_P_Pos_Output_Battery_Open();
			/* P106D00 Actuator Initial Drive Error */
			Fault_detect_ACT_Init_Error();
			/* P106D13 Actuator Hall IC Open */
			Fault_detect_Hall_IC_Open();
			/* P106D14 Motor UVW Open */
			Fault_detect_Motor_UVW_GND_Short();
			/* P106D71 ACT  Stuck */
			Fault_detect_ACT_Stuck();
			/* P106E16 HallIC Power Open */
			Fault_detect_HallIC_Power_Open();
			/* P106E17 HallIC Power Short */
			Fault_detect_HallIC_Power_Short();
		}
		else{

		}
		CAN_Failsafe_Management();
		Check_Vehicle_Battery_Over_Voltage();
		Check_Vehicle_Battery_Under_Voltage();
		Check_Vehicle_Battery_immediat_Under_Voltage();
	}
	else{
		Apl_SCU11_CAN.SCU_WarnMsg = 0u;
	}
	CAN_Fault_Reaction();
	SCU_Warning_Fault();
	Motor_Stop_Control();

}

/********************************************************************************************
| Project Name: Aurix TC2xx
| File Name: Fail_Safe.c
| Func Name: Check_Vehicle_Battery
| Func Type: static void
| Return Type : void
| DESCRIPTION: Check Vehicle Battery Voltage Level
|
| Output: Pwr_Vehicle_BAT
| Output Type : extern uint8
| 0U : BAT_Normal : Battery Voltage Normal : 9v ~ 16v
| 1U : BAT_LOW : Battery Voltage Low : ~ 8.5v
| 2U : BAT_High : Battery Voltage High : 16.5v ~
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00     hwkim    Creation
|********************************************************************************************/
static void Check_Vehicle_Battery_Under_Voltage(void)
{
	static uint16 Under_Counter = 0U;
	static uint16 Under_Reset_Counter = 0U;

	if((Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == 1U) && (g_Timeout_EMS_H12_0_RxTimeout == 0U) && (AI.VIN_Motor <= MT_BAT_78v)){
		if(Under_Counter >= Voltage_Check_Time){
			Pwr_Vehicle_Under_BAT = ON;
		}else{
			Under_Counter++;
		}
		Under_Reset_Counter = 0U;
	}else if(AI.VIN_Motor >= MT_BAT_80v){
		if(Pwr_Vehicle_Under_BAT == ON){
			if(Under_Reset_Counter >= Voltage_Reset_Time){
				Pwr_Vehicle_Under_BAT = OFF;
			}else{
				Under_Reset_Counter++;
			}
		}else{
			Under_Reset_Counter = 0U;
		}
		Under_Counter = 0U;
	}else{
		Under_Counter = 0U;
		Under_Reset_Counter = 0U;
	}

}

static void Check_Vehicle_Battery_Over_Voltage(void)
{
	static uint16 Over_Counter = 0U;
	static uint16 Over_Reset_Counter = 0U;

	if((Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == 1U) && (g_Timeout_EMS_H12_0_RxTimeout == 0U) && (AI.VIN_Motor >= MT_BAT_165v)){
		if(Over_Counter >= Voltage_Check_Time){
			Pwr_Vehicle_Over_BAT = ON;
		}else{
		Over_Counter++;
		}
		Over_Reset_Counter = 0U;
	}else if(AI.VIN_Motor <= MT_BAT_160v){
		if(Pwr_Vehicle_Over_BAT == ON){
			if(Over_Reset_Counter >= Voltage_Reset_Time){
				Pwr_Vehicle_Over_BAT = OFF;
			}else{
				Over_Reset_Counter++;
			}
		}else{
		Over_Reset_Counter = 0U;
		}
		Over_Counter = 0U;
	}else{
		Over_Counter = 0U;
		Over_Reset_Counter = 0U;
	}


}


static void Check_Vehicle_Battery_immediat_Under_Voltage(void)
{
	static uint16 Immediate_Under_Counter = 0U;
	static uint16 Immediate_Under_Reset_Counter = 0U;

	if((Apl_EMS_H12_CAN.CF_Ems_EngRunNorm == 1U) && (g_Timeout_EMS_H12_0_RxTimeout == 0U) && (AI.VIN_Motor <= MT_BAT_75v)){
		if(Immediate_Under_Counter >= 5){
			Immediate_Under_BAT = ON;
		}else{
			Immediate_Under_Counter++;
		}
		Immediate_Under_Reset_Counter = 0U;
	}else if(AI.VIN_Motor >= MT_BAT_80v){
		if(Immediate_Under_BAT == ON){
			if(Immediate_Under_Reset_Counter >= Voltage_Reset_Time){
				Immediate_Under_BAT = OFF;
			}else{
				Immediate_Under_Reset_Counter++;
			}
		}else{
			Immediate_Under_Reset_Counter = 0U;
		}
		Immediate_Under_Counter = 0U;
	}else{
		Immediate_Under_Counter = 0U;
		Immediate_Under_Reset_Counter = 0U;
	}

}

/*********************************************************************************************************************
* Function Name :  Fault_detect_EEPROM
**********************************************************************************************************************
* DESCRIPTION : P060141 EEPROM Fault Checksum
**********************************************************************************************************************/
void Fault_detect_EEPROM(void){


}

/*********************************************************************************************************************
* Function Name :  Fault_detect_Pos_Sens_Signal_Short
**********************************************************************************************************************
* DESCRIPTION :  P070512 Position Sensor Signal Short
**********************************************************************************************************************/
void Fault_detect_Pos_Sens_Signal_Short(void){

	static uint16 sens_short_error_cnt = 0u;
	static uint16 sens_short_error_cnt_2 = 0u;
	static uint8 sens_short_recovery_cnt = 0u;

	if ((s32_CurrentShaftPostion_A >= 99)){
		if (sens_short_error_cnt > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag = 1u;
			fgDTC_flag_Position_Sensor_Battery_Short = 1;
		} else {
			/* Error Flag Clear */
			sens_short_error_cnt++;
		}

	}
	else {
		sens_short_error_cnt = 0u;
	}

	if ((s32_CurrentShaftPostion_B >= 99)) {
		if (sens_short_error_cnt_2 > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag = 1u;
			fgDTC_flag_Position_Sensor_Battery_Short = 1;

		} else {
			/* Error Flag Clear */
			sens_short_error_cnt_2++;
		}

	} else {
		sens_short_error_cnt_2 = 0u;
	}

	if(SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag ==1)
	{
		if((s32_CurrentShaftPostion_A >=10 && s32_CurrentShaftPostion_A <=90)
				&&(s32_CurrentShaftPostion_B >=10 && s32_CurrentShaftPostion_B <=90) )
		{
			if(sens_short_recovery_cnt>10)
			{
				sens_short_recovery_cnt = 0u;
				SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag = 0u;
				fgDTC_flag_Position_Sensor_Battery_Short = 0;

			}
			else{
				sens_short_recovery_cnt++;
			}

		}
		else{
			sens_short_recovery_cnt = 0u;
		}
	}
	else{
		sens_short_recovery_cnt  = 0u;
	}
}

/*********************************************************************************************************************
* Function Name :  Fault_detect_Pos_Sens_Signal_Open
**********************************************************************************************************************
* DESCRIPTION :  P070514 Position Sensor Signal Open
**********************************************************************************************************************/
void Fault_detect_Pos_Sens_Signal_Open(void){


	static uint16 sens_open_error_cnt = 0u;
	static uint16 sens_open_error_cnt_2 = 0u;
	static uint8 sens_open_recovery_cnt = 0u;

	if ((s32_CurrentShaftPostion_A<=1)){
		if (sens_open_error_cnt > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag = 1u;
			fgDTC_flag_Position_Sensor_Circuit_Short = 1;
		} else {
			/* Error Flag Clear */
			sens_open_error_cnt++;
		}

	}
	else {
		sens_open_error_cnt = 0u;
	}

	if ((s32_CurrentShaftPostion_B<=1)) {
		if (sens_open_error_cnt_2 > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag = 1u;
			fgDTC_flag_Position_Sensor_Circuit_Short = 1;

		} else {
			/* Error Flag Clear */
			sens_open_error_cnt_2++;
		}

	} else {
		sens_open_error_cnt_2 = 0u;
	}

	if(SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag ==1)
	{
		if((s32_CurrentShaftPostion_A >=10 && s32_CurrentShaftPostion_A <=90)
				&&(s32_CurrentShaftPostion_B >=10 && s32_CurrentShaftPostion_B <=90) )
		{
			if(sens_open_recovery_cnt>10)
			{
				sens_open_recovery_cnt = 0u;
				SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag = 0u;
				fgDTC_flag_Position_Sensor_Circuit_Short = 0;

			}
			else{
				sens_open_recovery_cnt++;
			}

		}
		else{
			sens_open_recovery_cnt = 0u;
		}
	}
	else{
		sens_open_recovery_cnt  = 0u;
	}
}

/* P070516 Position Sensor Power Open */
void Fault_detect_Pos_Sens_Power_Open(void){
	/* To Be Detect  */
	fgDTC_flag_Position_Sensor_Power_Short = 0;
}

/* P070517 Position Sensor Power Short */
void Fault_detect_Pos_Sens_Power_Short(void){
	/* To Be Detect  */
	fgDTC_flag_Position_Sensor_Power_Battery_Short = 0;
}


/* P070661 Position Sensor Data Error */
void Fault_detect_Pos_Sens_Data_Error(void)
{
	static uint16 sens_sum_error_cnt= 0u;
	static uint8 sens_sum_recovery_cnt = 0u;
	sint32 sensor_sum_data = 0u;

	sensor_sum_data = s32_CurrentShaftPostion_A - (100-s32_CurrentShaftPostion_B);
	if((sensor_sum_data >= -3) && (sensor_sum_data <3))
	{
		sens_sum_error_cnt = 0;
		if(SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag == 1u)
		{
			if(sens_sum_recovery_cnt>10)
			{
				SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag = 0u;
				sens_sum_recovery_cnt = 0u;
			}
			else{
				sens_sum_recovery_cnt++;
			}
		}
		else{

		}
	}
	else{
		if (sens_sum_error_cnt > 600)
		{
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag = 1u;
			fgDTC_flag_Position_Sensor_Signal_Error = 1;
		}
		else {
			/* Error Flag Clear */
			SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag = 0u;
			fgDTC_flag_Position_Sensor_Signal_Error = 0;
			sens_sum_error_cnt++;
		}
		sens_sum_recovery_cnt = 0u;
	}

}

/* P070696 Position Sensor Output Error */
void Fault_detect_Pos_Sens_Output_Error(void)
{
	static uint16 sens_data_error_cnt = 0u;
	static uint16 sens_data_error_cnt_2 = 0u;
	static uint8 sens_data_recovery_cnt = 0u;

	if((s32_CurrentShaftPostion_A>91) || (s32_CurrentShaftPostion_A<9))
	{
		if (sens_data_error_cnt > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag = 1u;
			fgDTC_flag_Position_Sensor_Output_Error = 1;
		} else {
			/* Error Flag Clear */
			sens_data_error_cnt++;
		}

	}
	else {
		sens_data_error_cnt = 0u;
	}

	if ((s32_CurrentShaftPostion_B>91) ||(s32_CurrentShaftPostion_B<9)) {
		if (sens_data_error_cnt_2 > 600) {
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag = 1u;
			fgDTC_flag_Position_Sensor_Output_Error = 1;
		} else {
			/* Error Flag Clear */
			sens_data_error_cnt_2++;
		}

	} else {
		sens_data_error_cnt_2 = 0u;
	}

	if(SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag ==1)
	{
		if((s32_CurrentShaftPostion_A >=10 && s32_CurrentShaftPostion_A <=90)
				&&(s32_CurrentShaftPostion_B >=10 && s32_CurrentShaftPostion_B <=90))
		{
			if(sens_data_recovery_cnt>10)
			{
				sens_data_recovery_cnt = 0u;
				SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag = 0u;
				fgDTC_flag_Position_Sensor_Output_Error = 0;

			}
			else{
				sens_data_recovery_cnt++;
			}

		}
		else{
			sens_data_recovery_cnt = 0u;
		}
	}
	else{
		sens_data_recovery_cnt  = 0u;
	}
}


/* P106114 Motor Battery Open */
void Fault_detect_Motor_Battery_Open(void)
{
	static uint8 motor_battery_open_cnt = 0u;
	static uint8 motor_battery_open_recovery_cnt = 0u;
	V_BAT_MOTOR_SENSE = AI.VIN_Motor ;

	if (IGN_STAT == ON && V_BAT_SENSE > 966)
	{
		if (V_BAT_MOTOR_SENSE < 726U)
		{
			if (motor_battery_open_cnt > 100) /* 1sec */
			{
				/* Error Flag Clear */
				SCU_FAULT_State_Flag.Motor_Battery_Open_flag = 1U;
				fgDTC_flag_SBW_Motor_Power_Short = 1;
			}
			else
			{
				/* Error Flag Clear */
				motor_battery_open_cnt++;
			}

		}
		else
		{
			motor_battery_open_cnt = 0u;
		}
	}


	if(SCU_FAULT_State_Flag.Motor_Battery_Open_flag ==1)
	{
		if(V_BAT_MOTOR_SENSE >=900)
		{
			motor_battery_open_recovery_cnt++;
		}
		else{
			motor_battery_open_recovery_cnt = 0u;
		}
		if(motor_battery_open_recovery_cnt>10)
		{
			SCU_FAULT_State_Flag.Motor_Battery_Open_flag = 0u;
			fgDTC_flag_SBW_Motor_Power_Short = 0;
		}
		else{

		}
	}
}


/* P106012 Motor UVW Battery Short */
void Fault_detect_Motor_UVW_Short(void)
{
	static uint8 uvw_battery_short_cnt = 0u;
	static uint8 uvw_battery_short_recovery_cnt = 0u;

	if((AI.CS_1<1324) || (AI.CS_2<1324))
	{
		if(uvw_battery_short_cnt>5 )
		{
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Motor_UVW_Short_flag = 1U;
			fgDTC_flag_SBW_Motor_Battery_Short = 1;
		}
		else{
			/* Error Flag Clear */
			/*SCU_FAULT_State_Flag.Motor_UVW_Short_flag = 0U;*/

			uvw_battery_short_cnt++;
		}
	}
	else{
		uvw_battery_short_cnt = 0;
	}
	if(SCU_FAULT_State_Flag.Motor_UVW_Short_flag == 1U)
	{
		if((AI.CS_1>=1324) && (AI.CS_2>=1324) )
		{
			if(uvw_battery_short_recovery_cnt>10)
			{
				SCU_FAULT_State_Flag.Motor_UVW_Short_flag  = 0u;
				fgDTC_flag_SBW_Motor_Battery_Short = 0;
			}
			else{
				uvw_battery_short_recovery_cnt++;
			}

		}
	}


}


/* P106400 Motor Motor Over Current */
void Fault_detect_Motor_Over_Current(void)
{
	static uint8 uvw_battery_over_current_cnt = 0u;
	static uint8 uvw_battery_over_current_recovery_cnt = 0u;


	if((AI.CS_1<1000) || (AI.CS_2<1000))
	{
		if(uvw_battery_over_current_cnt>5 )
		{
			/* Error Flag Set */
			SCU_FAULT_State_Flag.Motor_Over_Current_flag = 1U;
			fgDTC_flag_SBW_Motor_Over_Current = 1;
		}
		else{
			/* Error Flag Clear */
			uvw_battery_over_current_cnt++;
		}
		uvw_battery_over_current_recovery_cnt = 0u;
	}
	else{
		if(SCU_FAULT_State_Flag.Motor_Over_Current_flag == 1u)
		{
			if(uvw_battery_over_current_recovery_cnt>10)
			{
				SCU_FAULT_State_Flag.Motor_Over_Current_flag = 0U;
				fgDTC_flag_SBW_Motor_Over_Current = 0;
				uvw_battery_over_current_recovery_cnt=0u;
			}
			else{
				uvw_battery_over_current_recovery_cnt++;
			}
		}
		uvw_battery_over_current_cnt = 0;
	}
}


static void Shifting_Detect(void){

	static uint8 first_check_flag = 0u;

	if(((SCU_Tar_Position != pre_task_tar_pos) && (first_check_flag==0u)))
	{
		pre_s32_CurrentShaftPostion = s32_CurrentShaftPostion_Ref;
		pre_f32_HallIC_Position = f32_Position_Real;
		detect_Motor_Move_flag = 1u;
		first_check_flag  = 1u;
	}
	else{

	}
	/* - 모터 위치 변화 시작 시점부터 모터 위치 변화량이 10 카운트 이상인 경우
	 * - 모터 위치 변화 시작 시점부터 포지션 센서 값 변화량이 5% 미만인 경우
	 */
	if(detect_Motor_Move_flag==1U)
	{
		if(Apl_SCU11_CAN.SCU_POS_ACT == Apl_SCU11_CAN.SCU_POS_TAR)
		{

			if(delta_Inhibit_Postion > 5 || delta_Inhibit_Postion < -5 )
			{
				detect_InhibitSw_Move_flag = 1U;
			}
			else{
				detect_InhibitSw_Move_flag = 0U;
			}

			if(delta_HallIC_Position> 10 || delta_HallIC_Position < -10)
			{
				detect_Hall_IC_Move_flag = 1u;
			}
			else{
				detect_Hall_IC_Move_flag = 0u;
			}
			pre_s32_CurrentShaftPostion = s32_CurrentShaftPostion;
			pre_f32_HallIC_Position = f32_Position_Real;
			detect_Motor_Move_flag = 0U;
			first_check_flag  = 0u;
		}
	}
	else{

	}
	delta_Inhibit_Postion = s32_CurrentShaftPostion - pre_s32_CurrentShaftPostion;
	delta_HallIC_Position = f32_Position_Real - pre_f32_HallIC_Position;
	/*pre_s32_CurrentShaftPostion_Ref = s32_CurrentShaftPostion_Ref;*/

}



uint8 motor_System_Recovery_cnt = 0u;
static uint8 motor_System_Error_cnt = 0u;
/* P106800 Motor System Error */
void Fault_detect_Motor_System_error(void)
{

	static uint8 pre_detect_Motor_Move_flag = 0u;

	/*
	 *
	 * - 모터 위치 변화 시작 시점부터 모터 위치 변화량이 10 카운트 이상인 경우
	 * - 모터 위치 변화 시작 시점부터 포지션 센서 값 변화량이 5% 미만인 경우
	 */

	/*if(detect_Motor_Move_flag == 0 && pre_detect_Motor_Move_flag == 1)*/
	if(1)
	{
		if((detect_Hall_IC_Move_flag == 1u) && (detect_InhibitSw_Move_flag == 0u))
		{
			motor_System_Error_cnt++;
		}
		else if((detect_Hall_IC_Move_flag == 1u) && (detect_InhibitSw_Move_flag == 1u))
		{
			motor_System_Recovery_cnt++;

			motor_System_Error_cnt =0u;
			SCU_FAULT_State_Flag.Motor_System_error_flag = 0U;
			fgDTC_flag_SBW_Motor_System_Error = 0u;
		}
		else{

		}
	}

	if(motor_System_Error_cnt>20)
	{
		//SCU_FAULT_State_Flag.Motor_System_error_flag = 1U;
		//fgDTC_flag_SBW_Motor_System_Error = 1u;
		//motor_System_Error_cnt = 21;
	}
	else{

	}

	pre_detect_Motor_Move_flag = detect_Motor_Move_flag;
}

/* P106900 Motor Operate Error */
void Fauilt_detect_Motor_Operate_error(void)
{

	if(s16_PwmDuty == 0){

	}
	static uint16 Motor_Operate_error_cnt = 0u;
	static uint8 Motor_Operate_recovery_cnt = 0u;


	if((f32_Position_Real>80) || (f32_Position_Real<-80))
	{
		Motor_Operate_recovery_cnt = 0u;
		if(Motor_Operate_error_cnt>3000u)
		{
			SCU_FAULT_State_Flag.Motor_Operate_error_flag = 1u;
		}
		else{
			Motor_Operate_error_cnt++;
		}
	}
	else
	{
		Motor_Operate_error_cnt = 0u;
		if(Motor_Operate_recovery_cnt>100)
		{
			SCU_FAULT_State_Flag.Motor_Operate_error_flag = 0u;
		}
		else{
			Motor_Operate_recovery_cnt++;
		}

	}
}

/* P106A12 HallIC_Battery_Short */
void Fault_detect_HallIC_Battery_Short(void)
{
	static uint8 hallic_bat_short_fault_cnt = 0u;
	static uint8 hallic_bat_short_recovery_cnt = 0u;

	if ((ui16_Hall_U_mcu == 1u) && (ui16_Hall_V_mcu == 1u) && (ui16_Hall_W_mcu == 1u)) {
		if (hallic_bat_short_fault_cnt++ > 100U) {
			SCU_FAULT_State_Flag.HallIC_Battery_Short_flag = 1u;
			fgDTC_flag_SBW_Motor_HallIC_Battery_Short = 1;
			hallic_bat_short_fault_cnt = 100U;
		}
		hallic_bat_short_recovery_cnt = 0u;
	}
	else {

		if (SCU_FAULT_State_Flag.HallIC_Battery_Short_flag == 1u) {
			if (hallic_bat_short_recovery_cnt > 10) {
				SCU_FAULT_State_Flag.HallIC_Battery_Short_flag = 0u;
				fgDTC_flag_SBW_Motor_HallIC_Battery_Short = 0;
				hallic_bat_short_recovery_cnt = 0u;
			} else {
				hallic_bat_short_recovery_cnt++;
			}
		}
		hallic_bat_short_fault_cnt = 0U;
	}
}

/* P106B00 Gate Driver Error */
void Fault_detect_GateDriver_Error(void)
{
	static uint16 GateDriver_Error_cnt = 0u;
	static uint16 GateDriver_recovery_cnt = 0u;

	if(gate_driver_read_data == 0x7ffu)
	{
		GateDriver_recovery_cnt = 0u;
		if(GateDriver_Error_cnt>3000)
		{
			SCU_FAULT_State_Flag.GateDriver_Error_flag = 1u;
		}
		else
		{
			GateDriver_Error_cnt++;
		}
	}
	else{
		GateDriver_Error_cnt = 0u;
		if(GateDriver_recovery_cnt>3000)
		{
			SCU_FAULT_State_Flag.GateDriver_Error_flag = 0u;
		}
		else
		{
			GateDriver_recovery_cnt++;
		}
	}
}


/* P106C12 P Position Output Battery Short */
void Fault_detect_P_Pos_Output_Battery_Short(void)
{
		/*TBD*/
}

/* P106C14 P Position Output Battery Open */
void Fault_detect_P_Pos_Output_Battery_Open(void)
{
		/*TBD*/
}

/* P106D00 Actuator Initial Drive Error */
void Fault_detect_ACT_Init_Error(void)
{
		/*TBD*/
}

/* P106D13 Actuator Hall IC Open */
void Fault_detect_Hall_IC_Open(void)
{
#if 0
	static uint8 hallic_open_fault_cnt = 0u;
	static uint8 hallic_open_recovery_cnt = 0u;

	if ((ui16_Hall_U_mcu == 0u)
		&& (ui16_Hall_V_mcu == 0u)
		&& (ui16_Hall_W_mcu == 0u)) {
		if (hallic_open_fault_cnt++ > 100U) {
			SCU_FAULT_State_Flag.Hall_IC_Open_flag = 1u;
			fgDTC_flag_SBW_Motor_Stuck_HallIC_Open_Short = 1;
			hallic_open_fault_cnt = 100U;
		}
		hallic_open_recovery_cnt = 0u;
	}
	else {

		if (SCU_FAULT_State_Flag.Hall_IC_Open_flag == 1u)
		{
			if (hallic_open_recovery_cnt > 10)
			{
				SCU_FAULT_State_Flag.Hall_IC_Open_flag = 0u;
				fgDTC_flag_SBW_Motor_Stuck_HallIC_Open_Short = 0;
				hallic_open_recovery_cnt = 0u;
			}
			else
			{
				hallic_open_recovery_cnt++;
			}
		}
		hallic_open_fault_cnt = 0U;
	}
#else
	if(hall_ic_pattern_error_cnt> 99)
	{
		hall_ic_pattern_error_flag = 1u;
		hall_ic_pattern_error_cnt = 100;
		SCU_FAULT_State_Flag.Hall_IC_Open_flag = 1u;
		fgDTC_flag_SBW_Motor_Stuck_HallIC_Open_Short = 1;
	}
	else{

	}

	if((hall_ic_pattern_recovery_cnt>99) && (SCU_FAULT_State_Flag.Hall_IC_Open_flag == 1u))
	{
		hall_ic_pattern_recovery_cnt = 0u;
		hall_ic_pattern_error_flag = 0u;
		hall_ic_pattern_error_cnt = 0;
		SCU_FAULT_State_Flag.Hall_IC_Open_flag = 0u;
		fgDTC_flag_SBW_Motor_Stuck_HallIC_Open_Short = 0;
	}

#endif
}

/* P106D14 Motor UVW Open */
void Fault_detect_Motor_UVW_GND_Short(void)
{

}

/* P106D71 ACT  Stuck */
void Fault_detect_ACT_Stuck(void)
{
	static uint8 motor_stuck_occur = 0u;
	static uint16 Motor_stuck_fault_cnt = 0u;
	static uint16 Motor_stuck_recovery_cnt = 0u;

	if(s32_ShaftPosition_Pre <-5 || s32_ShaftPosition_Pre >5 )
	{
		if(Motor_stuck_fault_cnt>800u){
			motor_stuck_occur++;
			Motor_stuck_fault_cnt = 0u;

		}
		else{
			SCU_FAULT_State_Flag.ACT_Stuck_flag = 0u;
			fgDTC_flag_SBW_Motor_Stuck = 0;
			Motor_stuck_fault_cnt++;
		}
		Motor_stuck_recovery_cnt = 0u;
	}
	else{
		Motor_stuck_fault_cnt = 0U;
		motor_stuck_occur = 0;
	}

	if(motor_stuck_occur>3)
	{
		motor_stuck_flag = 1u;
	}
	else if(motor_stuck_occur<2) {
		motor_stuck_flag = 0u;

	}

	if(motor_stuck_occur>10)
	{
		SCU_FAULT_State_Flag.ACT_Stuck_flag = 1u;
		fgDTC_flag_SBW_Motor_Stuck = 1;
	}
	else{

	}


	if (SCU_FAULT_State_Flag.ACT_Stuck_flag == 1u) {
		if ((s32_ShaftPosition_Pre >= -5) && (s32_ShaftPosition_Pre <= 5)) {
			if (Motor_stuck_recovery_cnt > 100) {
				motor_stuck_occur = 0;
				Motor_stuck_recovery_cnt = 0u;
				SCU_FAULT_State_Flag.ACT_Stuck_flag = 0u;
				fgDTC_flag_SBW_Motor_Stuck = 0;
			} else {
				Motor_stuck_recovery_cnt++;
			}
		}
	}
}

/* P106E16 HallIC Power Open */
void Fault_detect_HallIC_Power_Open(void)
{

	/* Reason HW Issue - Not implement circuit */
	fgDTC_flag_SBW_Motor_HallIC_Power_Open = 0;
}

/* P106E17 HallIC Power Short */
void Fault_detect_HallIC_Power_Short(void)
{
	/* Reason HW Issue - Not implement circuit */
	fgDTC_flag_SBW_Motor_HallIC_Power_Short = 0;
}


static void SCU_Warning_Fault(void){

	if((Apl_SCU11_CAN.SCU_WarnMsg == 1u) || (Apl_SCU11_CAN.SCU_WarnMsg == 9u))
	{
		SCU_Fault_flag = 1u;

	}
	else{
		SCU_Fault_flag = 0u;
	}
}


static void Motor_Stop_Control(void){

	static uint16 stop_recovery_cnt = 0u;

	if(SCU_Fault_flag ==1)
	{
		if(stop_recovery_cnt>200)
		{
			control_pi = 0;
		}
		else{
			stop_recovery_cnt++;
		}
	}

	if((control_pi == 0u) && (SCU_Fault_flag==0u))
	{
		control_pi = 1u;
	}

}


static void CAN_Fault_Reaction(void){

	if((SCU_FAULT_State_Flag.P_CAN_EMS_Timeout_flag == 1u)
	||(SCU_FAULT_State_Flag.P_CAN_CGW_Timeout_flag == 1u)
	||(SCU_FAULT_State_Flag.P_CAN_ESC_Timeout_flag == 1u)
	||(SCU_FAULT_State_Flag.P_CAN_SBW_Timeout_flag == 1u)
	||(SCU_FAULT_State_Flag.P_CAN_TCU_Timeout_flag == 1u)
	||(SCU_FAULT_State_Flag.P_Can_Bus_Off_flag == 1u))
	{
		CAN_Fault_flag = 1u;
	}
	else{
		CAN_Fault_flag = 0u;
	}
}





