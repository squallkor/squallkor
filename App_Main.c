/********************************************************************************************
| Project Name: DE EV E-Shifter
|    File Name: App_Main.c
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
| shum          UmSungHun             SL Corporation
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


uint8 Apl_Motor_ON_En		= 0U;
uint8 Apl_Gate_Drv_En 		= 0U;
uint8 Apl_Drv_Off 			= 0U;
uint8 Apl_V5_Sensor_En 		= 1U;
uint8 Apl_Can_Stb 			= 1U;

uint8 Apl_Motor_Error 		= 0U;
uint8 Apl_SBC_Flt_Status 	= 0U;
uint8 Apl_SBC_Int_Status 	= 0U;

uint8 Apl_lvr_pos_tar 		= 0u;
uint8 Apl_Motor_Move_stat 	= 0u;	/* 0: Stop , 1:Moving*/

uint8 apl_init_complete_flag = 0u;
static uint8 apl_init_cnt = 0u;


tU32 POSITION_PWMIN_Duty;

tU32 POSITION_SENSOR_Total;
tU16 Master_Period  = 1600u;
tU16 Reference_Period  = 1600u;


tU16 Test_Duty1 = 1600;
tU16 Test_Duty2 = 1600;
tU16 Test_Duty3 = 1600;
tU16 Test_Duty4 = 1000;


Icu_17_GtmCcu6_DutyCycleType	POSITION_SENSOR_1;
Icu_17_GtmCcu6_DutyCycleType	POSITION_SENSOR_2;
Icu_17_GtmCcu6_DutyCycleType	INHIBIT_SW_1;
Icu_17_GtmCcu6_DutyCycleType	INHIBIT_SW_2;

tU32 POSITION_SENSOR_1_Duty;
tU32 POSITION_SENSOR_2_Duty;
tU32 INHIBIT_SW_1_Duty;
tU32 INHIBIT_SW_2_Duty;

gRoup0_ad AI;


static void Output_Management(void);
static void Input_Management(void);
static void Apl_Initialize(void);

void App_Main(void)
{
	Apl_Initialize();
	Input_Management();
	Power_Management();
	/*Motor_Manage_Main();*/  /*19.8.26*/
	SBC_Manage_Main();
	SCU_Management();
	Motor_Manage_Main();
	Output_Management();

}

static void Input_Management(void)
{

	CAN_MAIN_INPUT();
	Adc_StartGroupConversion(AdcConf_AdcGroup_Adc0_AutoScan);
	Adc_StartGroupConversion(AdcConf_AdcGroup_Adc1_AutoScan);

	AI.SENSOR_TEMP = Adc_Result_DataBufferPtr0[1];				/* SENSOR TEMP*/
	AI.IG1_IN =	Adc_Result_DataBufferPtr0[2];					/* IG*/
	AI.Brake_Light_Sw = Adc_Result_DataBufferPtr0[3];			/* Brake*/

	AI.VIN_Motor_On = Adc_Result_DataBufferPtr1[1];			/* VIN_MOTOR_ON*/
	AI.VIN_Motor = Adc_Result_DataBufferPtr1[2];			/* VIN_MOTOR*/
	AI.VIN_System = Adc_Result_DataBufferPtr1[3];			/* VIN_SYSTEM*/

	/*Apl_Motor_Error = Dio_ReadChannel(DioConf_DioChannel_MOTOR_ERR);  */   /*P11.12 1msec task */
	/*Apl_SBC_Flt_Status = Dio_ReadChannel(DioConf_DioChannel_SBC_FLT); */ /*P20.09*/
	/*Apl_SBC_Int_Status = Dio_ReadChannel(DioConf_DioChannel_SBC_INT);	*/ /*P20.10*/
	Apl_SBC_Flt_Status  = P20_IN.B.P9;			/* SBC Fault Low : Error , High : Normal */
	Apl_SBC_Int_Status = P20_IN.B.P10;			/* SBC Int   Low : Error , High : Normal */


}

void GPIO_OUTPUT_Control(void)
{
	Dio_WriteChannel(DioConf_DioChannel_MOTOR_DRVOFF, Apl_Drv_Off);			/*P11.8*/
	Dio_WriteChannel(DioConf_DioChannel_MOTOR_EN, Apl_Gate_Drv_En);			/*P11.11*/
	Dio_WriteChannel(DioConf_DioChannel_SENSOR_EN, Apl_V5_Sensor_En);	    /*P13.3*/
	Dio_WriteChannel(DioConf_DioChannel_LOCAL_CAN_STB, Apl_Can_Stb);		/*P20.8*/
	Dio_WriteChannel(DioConf_DioChannel_VIN_MOTOR_ON_EN, Apl_Motor_ON_En);	/*P11.2*/
}

static void Output_Management(void)
{
	if(VBAT_STAT == 1U)
	{
		GPIO_OUTPUT_Control();
	}

	CAN_MAIN_OUTPUT();
}

static void Apl_Initialize(void)
{
	if(VBAT_STAT == 1U && apl_init_complete_flag == 0U)
	{

		Apl_Motor_ON_En		= 1U;
		Apl_Gate_Drv_En 	= 1U;
		Apl_Drv_Off 		= 0U;
		Apl_V5_Sensor_En 	= 1U;
		Apl_Can_Stb 		= 0U;		/* BUG FIX CAN Backup Enable - Low Active . 19.10.21 */

		GPIO_OUTPUT_Control();

		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_P_POSITION_OUTPUT,4000U);	   		/* TOM0_CH11 : P02.3, 	TP141*/

		/* A Phase */
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_1_HS1_PWM,DUTY_OFF);	/* TOM0_CH01 : P21.3, 	TP69*/
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_2_LS1_PWM,DUTY_OFF);	/* TOM0_CH02 : P21.4, 	TP65*/

		/* B Phase*/
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_3_HS2_PWM,DUTY_OFF);	/* TOM0_CH03 : P33.7, 	*/
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_4_LS2_PWM,DUTY_OFF);	/* TOM0_CH04 : P00.0, 	TP41*/

		/* C Phase*/
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_5_HS3_PWM,DUTY_OFF);	/*TOM0_CH05 : P02.2, 	TP40*/
		Pwm_17_Gtm_SetDutyCycle(Pwm_17_GtmConf_PwmChannel_PwmChannel_6_LS3_PWM,DUTY_OFF);	/*TOM0_CH06 : P02.4, 	TP39*/

		if(apl_init_cnt>=3u)
		{
			f32_Position_Ref = 0;
			PI_Init();
			Speed_PI_Init();
			Current_PI_Init();
			apl_init_complete_flag = drv_init();				/* BMD DRV Q 3245 Initialize */
			Fault_Detect_Function_OFF();
			SBC_HS_ON_Control();
			Diag_Setting();

		}
		else{
			apl_init_cnt++;
		}
	}
	else{
		apl_init_cnt = 0u;
	}

}
