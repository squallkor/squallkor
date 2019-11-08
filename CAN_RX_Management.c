/*
 * CAN_Error_Management.C
 *
 *  Created on: 2019. 9. 30.
 *      Author: dhshin
 */

#include "include.h"
#define ENV_TEST_MODE 1
tU08 g_Timeout_EMS_H12_0_RxTimeout;			/*EMS_H12 MAIN*/
tU08 g_Timeout_EMS_H12_1_RxTimeout;			/*EMS_H12 BACKUP*/
tU08 g_Timeout_CGW1_0_RxTimeout;				/*CGW1 MAIN*/
tU08 g_Timeout_CGW1_1_RxTimeout;				/*CGW1 BACKUP*/
tU08 g_Timeout_CGW2_0_RxTimeout;				/*CGW2 MAIN*/
tU08 g_Timeout_CGW2_1_RxTimeout;				/*CGW2 BACKUP*/
tU08 g_Timeout_EPB11_0_RxTimeout;			/*EPB11 MAIN*/
tU08 g_Timeout_EPB11_1_RxTimeout;			/*EPB11 BACKUP*/
tU08 g_Timeout_ESP11_0_RxTimeout;			/*ESP11 MAIN*/
tU08 g_Timeout_ESP11_1_RxTimeout;			/*ESP11 BACKUP*/
tU08 g_Timeout_WHL_SPD11_0_RxTimeout;		/*WHP_SPD11 MAIN*/
tU08 g_Timeout_WHL_SPD11_1_RxTimeout;		/*WHP_SPD11 BACKUP*/
tU08 g_Timeout_WHL_PUL11_0_RxTimeout;		/*WHL_PUL11 MAIN*/
tU08 g_Timeout_WHL_PUL11_1_RxTimeout;		/*WHL_PUL11 BACKUP*/
tU08 g_Timeout_Lvr12_0_RxTimeout;			/*LVR12 MAIN*/
tU08 g_Timeout_Lvr12_1_RxTimeout;			/*LVR12 BACKUP*/
tU08 g_Timeout_EMS11_0_RxTimeout;				/*EMS11 MAIN*/
tU08 g_Timeout_EMS11_1_RxTimeout;				/*EMS11 BACKUP*/
tU08 g_Timeout_EMS12_0_RxTimeout;				/*EMS12 MAIN*/
tU08 g_Timeout_EMS12_1_RxTimeout;				/*EMS12 BACKUP*/
tU08 g_Timeout_ESP12_0_RxTimeout;				/*ESP12 MAIN*/
tU08 g_Timeout_ESP12_1_RxTimeout;				/*ESP12 BACKUP*/
tU08 g_Timeout_TCU11_0_RxTimeout;			/*TCU11 MAIN*/
tU08 g_Timeout_TCU11_1_RxTimeout;			/*TCU11 BACKUP*/
tU08 g_Timeout_TCU13_0_RxTimeout;			/*TCU13 MAIN*/
tU08 g_Timeout_TCU13_1_RxTimeout;			/*TCU13 BACKUP*/

tU08  all_can_timeout_flag = 0u;

static uint8 CRC_Input[7];

static void Check_SBW_Alive_Cnt(void);
static void Get_CAN_CF_Lvr_UnitStatus(void);
static void Get_CAN_CF_Lvr_LeverType(void);
static void Get_CAN_CF_Lvr_IndicatorStatus(void);
static void Get_CAN_CF_Lvr_P_Status(void);
static void Get_CAN_CF_Lvr_R_Status(void);
static void Get_CAN_CF_Lvr_N_Status(void);
static void Get_CAN_CF_Lvr_D_Status(void);
static void Get_CAN_CF_Lvr_Wrn(void);
static void Get_CAN_CF_But_AC(void);
static void Get_CAN_CF_Lvr_CRC(void);

static void Get_CAN_CF_Ems_EngRunNorm(void);
static void Get_CAN_CF_Ems_IsgStat(void);

static void Get_CAN_TCU_SLOPE(void);
static void Get_CAN_TCU_ALV_CNT(void);

static void Get_CAN_TCU_OBD(void);
static void Get_CAN_F_TCU(void);
static void Get_CAN_G_SEL_DISP(void);


static void Get_CAN_WHL_SPD_FL_0(void);
static void Get_CAN_WHL_SPD_FR_0(void);
static void Get_CAN_WHL_SPD_RL_0(void);
static void Get_CAN_WHL_SPD_RR_0(void);
static void Get_CAN_WHL_SPD_AliveCounter_LSB(void);
static void Get_CAN_WHL_SPD_AliveCounter_MSB(void);


static void Get_CAN_LONG_ACCEL(void);
static void Get_CAN_LONG_ACCEL_STAT(void);
static void Get_CAN_LONG_ACCEL_DIAG(void);

static void Get_CAN_WHL_SPD_AliveCounter_MSB(void);
static void Get_CAN_WHL_SPD_AliveCounter_LSB(void);
static void Get_CAN_AVH_STAT(void);
static void Get_CAN_EPB_FRC_ERR(void);
static void Get_CAN_EPB_STATUS(void);
static void Get_CAN_CF_Gway_AstDrSw(void);
static void Get_CAN_CF_Gway_DrvSeatBeltSw(void);
static void Get_CAN_CF_Gway_DrvDrSw(void);
static void Get_CAN_CF_Gway_RLDrSw(void);
static void Get_CAN_CF_Gway_RRDrSw(void);
static void Get_CAN_CF_Clu_SbwNModeReq(void);
static void Get_CAN_CF_Ems_EngRunNorm(void);
static void Get_CAN_F_N_ENG(void);
static void Get_CAN_N(void);
static void Get_CAN_BRAKE_ACT(void);
static void Get_CAN_PV_AV_CAN(void);
static void Get_CAN_WHL_DIR_FL(void);
static void CAN_Timeout_Check_Function(void);
static void Set_LVR12_CRC_Input_Data(void);
static void CAN_Input_P_CAN(void);
static void CAN_Input(void);

void CAN_MAIN_INPUT(void)
{
	CAN_Input();
	CAN_Timeout_Check_Function();
}

static void CAN_Input(void) {

	CAN_Input_P_CAN();
}
static void CAN_Input_P_CAN(void)
{

	/* LVR(SBW) CAN SIGNAL INPUT */
	Get_CAN_CF_Lvr_P_Status();
	Get_CAN_CF_Lvr_R_Status();
	Get_CAN_CF_Lvr_N_Status();
	Get_CAN_CF_Lvr_D_Status();
	Get_CAN_CF_Lvr_Wrn();
	Get_CAN_CF_But_AC();
	Get_CAN_CF_Lvr_CRC();
	Get_CAN_CF_Lvr_UnitStatus();
	Get_CAN_CF_Lvr_LeverType();
	Get_CAN_CF_Lvr_IndicatorStatus();
	Check_SBW_Alive_Cnt();
	Set_LVR12_CRC_Input_Data();

	/* Cluster22 CAN SIGNAL INPUT */
	Get_CAN_CF_Clu_SbwNModeReq();
	Get_CAN_CF_Ems_EngRunNorm();

	/* CGW1 CAN SIGNAL INPUT */
	Get_CAN_CF_Gway_AstDrSw();
	Get_CAN_CF_Gway_DrvSeatBeltSw();
	Get_CAN_CF_Gway_DrvDrSw();

	/* CGW2 CAN SIGNAL INPUT */
	Get_CAN_CF_Gway_RLDrSw();
	Get_CAN_CF_Gway_RRDrSw();

	/* WHL_SPD11 CAN SIGNAL INPUT */
	Get_CAN_WHL_SPD_AliveCounter_MSB();
	Get_CAN_WHL_SPD_AliveCounter_LSB();

	/* ESP11 CAN SIGNAL INPUT */
	Get_CAN_AVH_STAT();

	/* EPB11 CAN SIGNAL INPUT */
	Get_CAN_EPB_FRC_ERR();
	Get_CAN_EPB_STATUS();

	/* EMS11 CAN SIGNAL INPUT */
	Get_CAN_F_N_ENG();
	Get_CAN_N();

	/* EMS12 CAN SIGNAL INPUT */
	Get_CAN_BRAKE_ACT();
	Get_CAN_PV_AV_CAN();

	/* EMS_H12 CAN SIGNAL INPUT */
	Get_CAN_CF_Ems_EngRunNorm();
	Get_CAN_CF_Ems_IsgStat();

	/* TCU CAN SIGNAL INPUT */
	Get_CAN_TCU_OBD();
	Get_CAN_F_TCU();
	Get_CAN_G_SEL_DISP();

	Get_CAN_TCU_SLOPE();
	Get_CAN_TCU_ALV_CNT();

	/* WHL_SPD CAN SIGNAL INPUT */
	Get_CAN_WHL_SPD_FL_0();
	Get_CAN_WHL_SPD_FR_0();
	Get_CAN_WHL_SPD_RL_0();
	Get_CAN_WHL_SPD_RR_0();
	Get_CAN_WHL_SPD_AliveCounter_LSB();
	Get_CAN_WHL_SPD_AliveCounter_MSB();

	/* ESP12 CAN SIGNAL INPUT */
	Get_CAN_LONG_ACCEL();
	Get_CAN_LONG_ACCEL_STAT();
	Get_CAN_LONG_ACCEL_DIAG();

	/* WHL_PUD CAN SIGNAL INPUT */
	Get_CAN_WHL_DIR_FL();
}


static void Get_CAN_CF_Lvr_UnitStatus(void){

	if(IlGetCF_Lvr_UnitStatus_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_UnitStatus = Can_ReadSig_CF_Lvr_UnitStatus_0();

	}
	else{
		if(IlGetCF_Lvr_UnitStatus_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_UnitStatus = Can_ReadSig_CF_Lvr_UnitStatus_1();
		}
		else{
			/* Nothing */
		}


	}
}

static void Get_CAN_CF_Lvr_LeverType(void){

	if(IlGetCF_Lvr_UnitStatus_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_LeverType = Can_ReadSig_CF_Lvr_LeverType_0();

	}
	else{
		if(IlGetCF_Lvr_UnitStatus_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_LeverType = Can_ReadSig_CF_Lvr_LeverType_1();
		}
		else{
			/* Nothing */
		}


	}
}

static void Get_CAN_CF_Lvr_IndicatorStatus(void){

	if(IlGetCF_Lvr_UnitStatus_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_IndicatorStatus = Can_ReadSig_CF_Lvr_IndicatorStatus_0();

	}
	else{
		if(IlGetCF_Lvr_UnitStatus_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_IndicatorStatus = Can_ReadSig_CF_Lvr_IndicatorStatus_1();
		}
		else{
			/* Nothing */
		}


	}
}

static void Get_CAN_CF_Lvr_CRC(void){

	if(IlGetCF_Lvr_CRC_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_CRC = Can_ReadSig_CF_Lvr_CRC_0();

	}
	else{
		if(IlGetCF_Lvr_P_Status_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_CRC = Can_ReadSig_CF_Lvr_CRC_1();
		}
		else{
			/* Nothing */
		}

	}
}


static void Get_CAN_CF_But_AC(void){

	if(IlGetCF_But_AC_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_But_AC = Can_ReadSig_CF_But_AC_0();

	}
	else{
		if(IlGetCF_But_AC_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_But_AC = Can_ReadSig_CF_But_AC_1();
		}
		else{
			/* Nothing */
		}
	}
}

static void Get_CAN_CF_Lvr_Wrn(void){

	if(IlGetCF_But_AC_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_Wrn = IlGetCF_Lvr_Wrn_0RxTimeout();

	}
	else{
		if(IlGetCF_But_AC_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_Wrn = IlGetCF_Lvr_Wrn_1RxTimeout();
		}
		else{
			/* Nothing */
		}


	}
}


static void Get_CAN_CF_Lvr_P_Status(void){

	if(IlGetCF_Lvr_P_Status_0RxTimeout() == 0){
		Apl_LVR12_CAN.CF_Lvr_PButtonStatus = Can_ReadSig_CF_Lvr_P_Status_0();
		g_Timeout_Lvr12_0_RxTimeout = 0U;
	}
	else{
		if(IlGetCF_Lvr_P_Status_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_PButtonStatus = Can_ReadSig_CF_Lvr_P_Status_1();
			g_Timeout_Lvr12_1_RxTimeout = 0u;
		}
		else{
			Apl_LVR12_CAN.CF_Lvr_PButtonStatus = 0x2U;
			g_Timeout_Lvr12_1_RxTimeout = 1u;
		}

		g_Timeout_Lvr12_0_RxTimeout = 1U;
	}
}

static void Get_CAN_CF_Lvr_R_Status(void){
	if(IlGetCF_Lvr_R_Status_0RxTimeout() == 0)
	{
		Apl_LVR12_CAN.CF_Lvr_RButtonStatus = Can_ReadSig_CF_Lvr_R_Status_0();
	}
	else{
		if(IlGetCF_Lvr_R_Status_1RxTimeout() == 0){
			Apl_LVR12_CAN.CF_Lvr_RButtonStatus = Can_ReadSig_CF_Lvr_R_Status_1();
		}
		else{
			Apl_LVR12_CAN.CF_Lvr_RButtonStatus = 0x2U;
		}


	}

}

static void Get_CAN_CF_Lvr_N_Status(void){
	if(IlGetCF_Lvr_N_Status_0RxTimeout() == 0)
	{
		Apl_LVR12_CAN.CF_Lvr_NButtonStatus = Can_ReadSig_CF_Lvr_N_Status_0();
	}
	else{
		if(IlGetCF_Lvr_N_Status_1RxTimeout() == 0)
		{
			Apl_LVR12_CAN.CF_Lvr_NButtonStatus = Can_ReadSig_CF_Lvr_N_Status_1();
		}
		else{
			Apl_LVR12_CAN.CF_Lvr_NButtonStatus = 0x02;
		}
	}

}

static void Get_CAN_CF_Lvr_D_Status(void){
	if(IlGetCF_Lvr_D_Status_0RxTimeout() == 0)
	{
		Apl_LVR12_CAN.CF_Lvr_DButtonStatus = Can_ReadSig_CF_Lvr_D_Status_0();
	}else{
		if(IlGetCF_Lvr_D_Status_1RxTimeout() == 0)
		{
			Apl_LVR12_CAN.CF_Lvr_DButtonStatus = Can_ReadSig_CF_Lvr_D_Status_1();
		}
		else{
			Apl_LVR12_CAN.CF_Lvr_DButtonStatus = 0x02;
		}
	}
}

static void Get_CAN_CF_Ems_EngRunNorm(void){

	if(IlGetCF_Ems_EngRunNormRxTimeout() == 0)
	{

		#if ENV_TEST_MODE
		Apl_EMS_H12_CAN.CF_Ems_EngRunNorm = 1;
#else
		Apl_EMS_H12_CAN.CF_Ems_EngRunNorm = Can_ReadSig_CF_Ems_EngRunNorm();
#endif
		g_Timeout_EMS_H12_0_RxTimeout = 0U;
	}
	else{

		g_Timeout_EMS_H12_0_RxTimeout = 1U;
	}

}

static void Get_CAN_CF_Ems_IsgStat(void){
		if(IlGetCF_Ems_IsgStat_0RxTimeout() == 0)
		{
			Apl_EMS_H12_CAN.CF_Ems_IsgStat = Can_ReadSig_CF_Ems_IsgStat_0();

		}
		else{
			Apl_EMS_H12_CAN.CF_Ems_IsgStat = Can_ReadSig_CF_Ems_IsgStat_1();
		}

	}

static void Get_CAN_CF_Clu_SbwNModeReq(void){
	Apl_CLU22_CAN.CF_Clu_SbwNModeReq  = Can_ReadSig_CF_Clu_SbwNModeReq_0();
}

static void Get_CAN_CF_Gway_RRDrSw(void){
	if(IlGetCF_Gway_RRDrSw_0RxTimeout() == 0)
	{
		Apl_CGW2_CAN.CF_Gway_RRDrSw = Can_ReadSig_CF_Gway_RRDrSw_0();
		g_Timeout_CGW2_0_RxTimeout = 0U;
	}
	else{
		g_Timeout_CGW2_0_RxTimeout = 1U;
	}

}

static void Get_CAN_CF_Gway_RLDrSw(void){
	if(IlGetCF_Gway_RLDrSw_0RxTimeout() == 0)
	{
		Apl_CGW2_CAN.CF_Gway_RLDrSw = Can_ReadSig_CF_Gway_RLDrSw_0();

	}
	else{

	}

}


 static void Get_CAN_CF_Gway_DrvDrSw(void){
 	if(IlGetCF_Gway_DrvDrSw_0RxTimeout() == 0)
	{
		Apl_CGW1_CAN.CF_Gway_DrvDrSw = Can_ReadSig_CF_Gway_DrvDrSw_0();
		g_Timeout_CGW1_0_RxTimeout = 0u;

	}
	else{
		g_Timeout_CGW1_0_RxTimeout = 1u;

	}

}


static void Get_CAN_CF_Gway_DrvSeatBeltSw(void){
	if(IlGetCF_Gway_DrvSeatBeltSw_0RxTimeout() == 0)
	{
		Apl_CGW1_CAN.CF_Gway_DrvSeatBeltSw = Can_ReadSig_CF_Gway_DrvSeatBeltSw_0();

	}
	else{

	}

}
static void Get_CAN_CF_Gway_AstDrSw(void){
	if(IlGetCF_Gway_AstDrSw_0RxTimeout() == 0)
	{
		Apl_CGW1_CAN.CF_Gway_AstDrSw = Can_ReadSig_CF_Gway_AstDrSw_0();

	}
	else{

	}

}

static void Get_CAN_EPB_STATUS(void){
	if(IlGetEPB_STATUS_0RxTimeout() == 0)
	{
		Apl_EPB11_CAN.EPB_STATUS = Can_ReadSig_EPB_STATUS_0();
		g_Timeout_EPB11_0_RxTimeout = 0u;
	}
	else{
		Apl_EPB11_CAN.EPB_STATUS = 0x1;
		g_Timeout_EPB11_0_RxTimeout = 1u;
	}

}

static void Get_CAN_EPB_FRC_ERR(void){
	if(IlGetEPB_FRC_ERR_0RxTimeout() == 0)
	{
		Apl_EPB11_CAN.EPB_FRC_ERR = Can_ReadSig_EPB_FRC_ERR_0();

	}
	else{

	}

}

static void Get_CAN_AVH_STAT(void){
	if(IlGetAVH_STAT_0RxTimeout() == 0)
	{
		Apl_ESP11_CAN.AVH_STAT = Can_ReadSig_AVH_STAT_0();
		g_Timeout_ESP11_0_RxTimeout = 0u;

	}
	else{
		g_Timeout_ESP11_0_RxTimeout = 1u;

	}

}

static void Get_CAN_BRAKE_ACT(void){

	if(IlGetBRAKE_ACT_0RxTimeout() == 0)
	{
#if ENV_TEST_MODE
	Apl_EMS12_CAN.BRAKE_ACT = 2u;
#else
		Apl_EMS12_CAN.BRAKE_ACT = Can_ReadSig_BRAKE_ACT_0();
#endif
		g_Timeout_EMS12_0_RxTimeout = 0U;

	}
	else{

		if(IlGetBRAKE_ACT_1RxTimeout() == 0)
		{
			Apl_EMS12_CAN.BRAKE_ACT = Can_ReadSig_BRAKE_ACT_1();
		}
		else{
			Apl_EMS12_CAN.BRAKE_ACT = CAN_BRAKE_ON;
		}

		g_Timeout_EMS12_0_RxTimeout = 1U;
	}


}


static void Get_CAN_PV_AV_CAN(void){
	if(IlGetPV_AV_CAN_0RxTimeout() == 0)
	{
		Apl_EMS12_CAN.PV_AV_CAN = Can_ReadSig_PV_AV_CAN_0();

	}
	else{

	}
}


static void Get_CAN_F_N_ENG(void){
	if(IlGetF_N_ENG_0RxTimeout() == 0)
	{
		Apl_EMS11_CAN.F_N_ENG = Can_ReadSig_F_N_ENG_0();
		g_Timeout_EMS11_0_RxTimeout = 0u;
	}
	else{
		g_Timeout_EMS11_0_RxTimeout = 1u;
	}
}

static void Get_CAN_N(void){
	if(IlGetN_0RxTimeout() == 0)
	{
		#if ENV_TEST_MODE
		Apl_EMS11_CAN.N  = 3500;
		#else
		Apl_EMS11_CAN.N  = Can_ReadSig_N_0();

		#endif


	}
	else{

	}
}

static void Get_CAN_LONG_ACCEL(void){
	if(IlGetLONG_ACCEL_0RxTimeout() == 0)
	{
		Apl_ESP12_CAN.LONG_ACCEL = Can_ReadSig_LONG_ACCEL_0();
		g_Timeout_ESP12_0_RxTimeout = 0U;
	}
	else{
		g_Timeout_ESP12_0_RxTimeout = 1U;

	}
}

static void Get_CAN_LONG_ACCEL_STAT(void){
	if(IlGetLONG_ACCEL_STAT_0RxTimeout() == 0)
	{
		Apl_ESP12_CAN.LONG_ACCEL_STAT= Can_ReadSig_LONG_ACCEL_STAT_0();

	}
	else{

	}
}
static void Get_CAN_LONG_ACCEL_DIAG(void){
	if(IlGetLONG_ACCEL_DIAG_0RxTimeout() == 0)
	{
		Apl_ESP12_CAN.LONG_ACCEL_DIAG = Can_ReadSig_LONG_ACCEL_DIAG_0();

	}
	else{

	}
}

float slope_test = 0;

static void Get_CAN_TCU_SLOPE(void)
{

	if(IlGetSLOPE_TCU_0RxTimeout() == 0U)
	{
		/* (data-32)*0.5  */
		Apl_TCU13_CAN.SLOPE_TCU = Can_ReadSig_SLOPE_TCU_0();
		slope_test =(Apl_TCU13_CAN.SLOPE_TCU -32) *0.5;
		g_Timeout_TCU13_0_RxTimeout = 0U;
	}
	else{
		g_Timeout_TCU13_0_RxTimeout =  1U;
		if(IlGetSLOPE_TCU_1RxTimeout() == 0U)
		{
			Apl_TCU13_CAN.SLOPE_TCU = Can_ReadSig_SLOPE_TCU_1();
			g_Timeout_TCU13_1_RxTimeout = 0u;
		}
		else{
			Apl_TCU13_CAN.SLOPE_TCU = 52;
			g_Timeout_TCU13_1_RxTimeout = 1u;
		}
	}
}
static void Get_CAN_TCU_ALV_CNT(void){

	if(IlGetCF_Tcu_Alive3_0RxTimeout() == 0U)
	{
		Apl_TCU13_CAN.CF_Tcu_Alive3 = Can_ReadSig_CF_Tcu_Alive3_0();
		/*g_Timeout_TCU1_RxTimeout = 0U;*/
	}
	else{
		/*g_Timeout_TCU1_RxTimeout =  1U;*/
		if(IlGetCF_Tcu_Alive3_1RxTimeout() == 0U)
		{
			Apl_TCU13_CAN.CF_Tcu_Alive3 = Can_ReadSig_CF_Tcu_Alive3_1();

		}
		else{


		}
	}

}


static void Get_CAN_G_SEL_DISP(void){
	if(IlGetG_SEL_DISP_0RxTimeout() == 0U)
	{
		Apl_TCU11_CAN.G_SEL_DISP = Can_ReadSig_G_SEL_DISP_0();
		g_Timeout_TCU11_0_RxTimeout = 0U;
	}
	else{
		g_Timeout_TCU11_0_RxTimeout =  1U;
		if(IlGetG_SEL_DISP_1RxTimeout() == 0U)
		{
			Apl_TCU11_CAN.G_SEL_DISP = Can_ReadSig_G_SEL_DISP_1();
			g_Timeout_TCU11_1_RxTimeout = 0u;
		}
		else{
			Apl_TCU11_CAN.G_SEL_DISP = 0x9u;
			g_Timeout_TCU11_1_RxTimeout = 1u;

		}

	}
}

static void Get_CAN_F_TCU(void){
	if(IlGetG_SEL_DISP_0RxTimeout() == 0U)
	{
		Apl_TCU11_CAN.F_TCU = Can_ReadSig_F_TCU_0();
	}
	else{
		if(IlGetG_SEL_DISP_1RxTimeout() == 0U)
		{
			Apl_TCU11_CAN.F_TCU= Can_ReadSig_F_TCU_1();
		}
		else{

		}

	}
}

static void Get_CAN_TCU_OBD(void){
	if(IlGetG_SEL_DISP_0RxTimeout() == 0U)
	{
		Apl_TCU11_CAN.TCU_OBD= Can_ReadSig_TCU_OBD_0();
	}
	else{

	}
}


static void Get_CAN_WHL_SPD_FL_0(void){
	if(IlGetWHL_SPD_FL_0RxTimeout() == 0U)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_FL = Can_ReadSig_WHL_SPD_FL_0();
		g_Timeout_WHL_SPD11_0_RxTimeout = 0U;
	}
	else{
		if(IlGetWHL_SPD_FL_1RxTimeout() == 0U)
		{
			Apl_WHL_SPD11_CAN.WHL_SPD_FL = Can_ReadSig_WHL_SPD_FL_1();
		}
		else{
			g_Timeout_WHL_SPD11_0_RxTimeout = 1U;
		}
	}
}


static void Get_CAN_WHL_SPD_FR_0(void){
	if(IlGetWHL_SPD_FR_0RxTimeout() == 0U)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_FR = Can_ReadSig_WHL_SPD_FR_0();
	}
	else{
		if(IlGetWHL_SPD_FR_1RxTimeout() == 0U)
		{
			Apl_WHL_SPD11_CAN.WHL_SPD_FR = Can_ReadSig_WHL_SPD_FR_1();
		}
		else{

		}
	}
}


static void Get_CAN_WHL_SPD_RL_0(void){
	if(IlGetWHL_SPD_RL_0RxTimeout() == 0U)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_RL = Can_ReadSig_WHL_SPD_RL_0();
	}
	else{
		if (IlGetWHL_SPD_RL_1RxTimeout() == 0U)
		{
			Apl_WHL_SPD11_CAN.WHL_SPD_RL = Can_ReadSig_WHL_SPD_RL_1();
		}
		else {

		}

	}
}

static void Get_CAN_WHL_SPD_RR_0(void){
	if(IlGetWHL_SPD_RR_0RxTimeout() == 0U)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_RR = Can_ReadSig_WHL_SPD_RR_0();
	}
	else{
		if (IlGetWHL_SPD_RR_1RxTimeout() == 0U)
		{
			Apl_WHL_SPD11_CAN.WHL_SPD_RR = Can_ReadSig_WHL_SPD_RR_1();
		}
		else {

		}
	}
}


static void Get_CAN_WHL_SPD_AliveCounter_LSB(void){
	if(IlGetWHL_SPD_FL_0RxTimeout() == 0)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_AliveCounter_LSB = Can_ReadSig_WHL_SPD_AliveCounter_LSB_0();

	}
	else{

	}

}

static void Get_CAN_WHL_SPD_AliveCounter_MSB(void){
	if(IlGetWHL_SPD_FL_0RxTimeout() == 0)
	{
		Apl_WHL_SPD11_CAN.WHL_SPD_AliveCounter_MSB = Can_ReadSig_WHL_SPD_AliveCounter_MSB_0();

	}
	else{

	}
}

static void Check_SBW_Alive_Cnt(void){

	static uint8 pre_task_AC_data = 0u;

	if ((uint8) IlGetCF_But_AC_0RxTimeout() == 0u) {
		if (Apl_LVR12_CAN.CF_But_AC == 0u)
		{
			if (pre_task_AC_data == 15u)
			{
				lvr_ac_error_flag = 0u;
			}
			else
			{
				lvr_ac_error_flag = 1u;
			}
		} else {
			if (Apl_LVR12_CAN.CF_But_AC == (pre_task_AC_data + 1u))
			{
				lvr_ac_error_flag = 0u;
			}
			else
			{
				lvr_ac_error_flag = 1u;
			}
		}
		pre_task_AC_data = Apl_LVR12_CAN.CF_But_AC;
	}
}


static void Get_CAN_WHL_DIR_FL(void)
{
	if(IlGetWHL_DIR_FL_1RxTimeout() == 0u)
	{
		Apl_WHL_PUL11_CAN.WHL_DIR_FL = Can_ReadSig_WHL_DIR_FL_0();
	}
	else{
		g_Timeout_WHL_PUL11_0_RxTimeout =1;
	}

}



/********************************************************************************************
| Project Name: TMc SCU
| File Name: CAN_Management.c
| Func Name: CRC Input Data
| Func Type: static void
| Return Type : uint8
| DESCRIPTION:  CRC Input data
|
| Output: return CRC
| Output Type : return uint8
| 0x0 ~ 0xFF : Calculated Data
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2018-05-08  1.00     dhshin    Creation
|********************************************************************************************/


static void Set_LVR12_CRC_Input_Data(void)
{

	static uint8 sbw_crc_data= 0u;
	CRC_Input[0] = (uint8)((uint8)(Apl_LVR12_CAN.CF_Lvr_DButtonStatus <<6U) +
							(uint8)(Apl_LVR12_CAN.CF_Lvr_NButtonStatus <<4U) +
							(uint8)(Apl_LVR12_CAN.CF_Lvr_RButtonStatus <<2U) +
							(uint8)((Apl_LVR12_CAN.CF_Lvr_PButtonStatus) & 0xFF));
	CRC_Input[1] = (uint8)((uint8)(Apl_LVR12_CAN.CF_Lvr_PRelStat <<2U) +
							(uint8)((Apl_LVR12_CAN.CF_Lvr_UnLockButtonStatus)& 0xFF));
	CRC_Input[2] = (uint8)((Apl_LVR12_CAN.CF_Lvr_LeverType)& 0xFF);
	CRC_Input[3] = (uint8)((Apl_LVR12_CAN.CF_Lvr_UnitStatus)& 0xFF);
	CRC_Input[4] = (uint8)((Apl_LVR12_CAN.CF_Lvr_IndicatorStatus)& 0xFF);
	CRC_Input[5] = (uint8)((Apl_LVR12_CAN.CF_Lvr_Wrn)& 0xFF);
	CRC_Input[6] = (uint8)((Apl_LVR12_CAN.CF_But_AC)& 0xFF);

	sbw_crc_data = Calculate_CRC(CRC_Input);

	if(IlGetCF_Lvr_CRC_0RxTimeout()==0u)
	{
		if(Apl_LVR12_CAN.CF_Lvr_CRC == sbw_crc_data)
		{
			lvr_crc_error_flag = 0u;
		}
		else
		{
			lvr_crc_error_flag = 1u;
		}
	}

}


static void CAN_Timeout_Check_Function(void)
{
	if((g_Timeout_EMS_H12_0_RxTimeout ==1)
	&&(g_Timeout_CGW2_0_RxTimeout==1)
	&&(g_Timeout_CGW1_0_RxTimeout==1)
	&&(g_Timeout_EPB11_0_RxTimeout==1)
	&&(g_Timeout_ESP11_0_RxTimeout==1)
	/*&&(g_Timeout_WHL_PUL11_RxTimeout==1)*/
	&&(g_Timeout_WHL_SPD11_0_RxTimeout==1)
	&&(g_Timeout_Lvr12_0_RxTimeout==1)
	/*&&(g_Timeout_Lvr12_1_RxTimeout)*/
	&&(g_Timeout_EMS12_0_RxTimeout==1)
	&&(g_Timeout_EMS11_0_RxTimeout==1)
	&&(g_Timeout_ESP12_0_RxTimeout==1)
	&&(g_Timeout_TCU11_0_RxTimeout==1)
	/*&&(g_Timeout_TCU13_0_RxTimeout==1)*/
	/*&&(g_Timeout_TCU1_BACKUP_RxTimeout)*/
	)
	{
		all_can_timeout_flag = 1u;
	}
	else{
		all_can_timeout_flag = 0u;
	}

}

