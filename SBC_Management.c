/*
 * SBC_Management.c
 *
 *  Created on: 2019. 2. 21.
 *      Author: dhshin
 */


#include "include.h"

uint8 dtc_set_complete_flag = 0u;
uint8 can_tx_flag = 0u;
uint32 sleep_time = 500u;

static void decision_sleep_time(void);
static void ECU_State_Management(void);
static void SCU_Sleep_GPIO_Control(void);

void SBC_Manage_Main(void){

	decision_sleep_time();
	ECU_State_Management();

}


static void decision_sleep_time(void)
{
	/* N-Wash Mode time 30minute awake after sleep */
	if(n_standby_mode == 2)
	{
		sleep_time = 180000u;
	}
	/* None N-Wash Mode time 30sec awake after sleep	*/
	else{
		sleep_time = 3000u;
	}

}

static void SCU_Sleep_GPIO_Control(void){
	Apl_Motor_ON_En = 0U;
	Apl_Gate_Drv_En = 0U;
	Apl_Drv_Off = 0U;
	Apl_V5_Sensor_En = 0U;
	Apl_Can_Stb = 1U;				/* LOW aCTIVE */
	GPIO_OUTPUT_Control();

}

static void ECU_State_Management(void){

	static uint32 sbc_sleep_cnt = 0u;


	if(IGN_STAT == 0)
	{
		sbc_sleep_cnt++;
	}
	else
	{
		sbc_sleep_cnt=0;
	}


	if(sbc_sleep_cnt >= sleep_time)
	{
		CAN_IL_TX_STOP();
		SBC_NormalMode();
		can_tx_flag = 0u;


		if(all_can_timeout_flag==1)
		{
			SBC_HS_OFF_Control();
			Ecu_PowerDown();
			if(dtc_set_complete_flag==1)
			{

				SCU_Sleep_GPIO_Control();
				
				SBC_SleepMode();
			}

		}
		else{

		}
		sbc_sleep_cnt = sleep_time;
	}
	else{
		SBC_NormalMode();
		if (System_Power == 2u)
		{
			if(can_tx_flag==0u)
			{
				CAN_IL_TX_START();
				can_tx_flag = 1u;
			}

		}
		else if(System_Power == 0u)
		{

		}
		else
		{

		}
	}
}
