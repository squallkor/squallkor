/*
 * App_Diag_Management.c
 *
 *  Created on: 2019. 9. 26.
 *      Author: dhshin
 */

#include "include.h"

static void Set_DID_0100(void);
static void Set_DID_0101_0102(void);
static void Set_DID_0103(void);
static void Set_DID_0104(void);
static void Set_DID_0105(void);
static void Set_DID_0106(void);

static void Set_F180(void);
static void Set_F189(void);
static void Set_F192(void);
static void Set_F194(void);
static void Set_F195(void);
static void Set_F198(void);
static void Set_F19A(void);
static void Set_F1A0(void);


void Diag_Setting(void)
{
	Set_F180();
	Set_F189();
	Set_F192();
	Set_F194();
	Set_F195();
	Set_F198();
	Set_F19A();
	Set_F1A0();
}


void Set_DID_Data(void)
{
	Set_DID_0100();
	Set_DID_0101_0102();
	Set_DID_0103();
	Set_DID_0104();
	Set_DID_0105();
	Set_DID_0106();
}

static void Set_DID_0100(void)
{

	DID_0100.Data_0100[0]  = (uint8)(AI.VIN_System>>8U);					/* System Battery 	*/
	DID_0100.Data_0100[1]  = (uint8)(AI.VIN_System);
	DID_0100.Data_0100[2]  = (uint8)(AI.VIN_Motor>>8U);						/* Motor Battery 	*/
	DID_0100.Data_0100[3]  = (uint8)(AI.VIN_Motor);
	DID_0100.Data_0100[4]  = (uint8)(AI.VIN_Motor_On>>8);
	DID_0100.Data_0100[5]  = (uint8)(AI.VIN_Motor_On);
	DID_0100.Data_0100[6]  = (uint8)(AI.IG1_IN>>8U);
	DID_0100.Data_0100[7]  = (uint8)(AI.IG1_IN);
	DID_0100.Data_0100[8]  = (uint8)(AI.SENSOR_TEMP>>8U);
	DID_0100.Data_0100[9]  = (uint8)(AI.SENSOR_TEMP);
	DID_0100.Data_0100[10] = (uint8)(AI.Brake_Light_Sw>>8U);
	DID_0100.Data_0100[11] = (uint8)(AI.Brake_Light_Sw);
	DID_0100.Data_0100[12] = (uint8)(AI.CS_1>>8);
	DID_0100.Data_0100[13] = (uint8)(AI.CS_1);
	DID_0100.Data_0100[14] = (uint8)(AI.CS_2>>8);
	DID_0100.Data_0100[15] = (uint8)(AI.CS_2);
	DID_0100.Data_0100[16] = (uint8)(AI.AI_MOTOR_CURRENT_U>>8U);
	DID_0100.Data_0100[17] = (uint8)(AI.AI_MOTOR_CURRENT_U);
	DID_0100.Data_0100[18] = (uint8)(AI.AI_MOTOR_CURRENT_V>>8);
	DID_0100.Data_0100[19] = (uint8)(AI.AI_MOTOR_CURRENT_V);
	DID_0100.Data_0100[20] = (uint8)((uint8)(Apl_Motor_ON_En)|(uint8)(Apl_Gate_Drv_En<<1U)|(uint8)(Apl_V5_Sensor_En<<2U)|(uint8)(Apl_Can_Stb<<3U)|
			                 (uint8)(Apl_Drv_Off<<4U)|(uint8)(Apl_Motor_Error<<5U)|(uint8)(Apl_SBC_Flt_Status<<6U)|(uint8)(Apl_SBC_Int_Status<<7U));

}

static void Set_DID_0101_0102(void)

{

	 DID_0101.Data_0101[0] =(uint8)(drv3245_status.ic_status0.bit.OTW)|(drv3245_status.ic_status0.bit.SPI_OK<<1)
							|(drv3245_status.ic_status0.bit.BIST_STAT<<2)|(drv3245_status.ic_status0.bit.DRV_STAT<<3)
							|(drv3245_status.ic_status0.bit.VDS_STAT<<4)|(drv3245_status.ic_status0.bit.PVDD_OVFL<<5)
							 |(drv3245_status.ic_status0.bit.PVDD_UVFL<<6)|(drv3245_status.ic_status0.bit.FAULT<<7);


	 DID_0101.Data_0101[1] = (uint8)(drv3245_status.oc_vds_faults.all>>8U);



	 DID_0101.Data_0101[2] = (uint8)(drv3245_status.oc_vds_faults.bit.SNS_A_OCP)|(drv3245_status.oc_vds_faults.bit.SNS_B_OCP<<1)
							 |(drv3245_status.oc_vds_faults.bit.SNS_C_OCP<<2)|(drv3245_status.oc_vds_faults.bit.VDS_LC<<3)
							 |(drv3245_status.oc_vds_faults.bit.VDS_HC<<4)|(drv3245_status.oc_vds_faults.bit.VDS_LB<<5)
							 |(drv3245_status.oc_vds_faults.bit.VDS_HB<<6)|(drv3245_status.oc_vds_faults.bit.VDS_LA<<7);


	 DID_0101.Data_0101[3] = (uint8)(drv3245_status.ic_faults.all>>8U);

	 DID_0101.Data_0101[4] = (uint8)(drv3245_status.ic_faults.bit.VCPH_OVLO_ABS)|(drv3245_status.ic_faults.bit.AVDD_OVLO<<1)
							 |(drv3245_status.ic_faults.bit.AVDD_UVLO<<2)|(drv3245_status.ic_faults.bit.VCP_LSD_OVLO<<3)
							 |(drv3245_status.ic_faults.bit.VCP_LSD_UVLO<<4)|(drv3245_status.ic_faults.bit.AVDD_UVLO<<5)
							 |(drv3245_status.ic_faults.bit.AVDD_OVLO<<6)|(drv3245_status.ic_faults.bit.DVDD_OVLO<<7);


	 DID_0101.Data_0101[5] = (uint8)(drv3245_status.vgs_faults.bit.VGS_LC)|(drv3245_status.vgs_faults.bit.VGS_HC<<1)
							 |(drv3245_status.vgs_faults.bit.VGS_LB<<2)|(drv3245_status.vgs_faults.bit.VGS_HB<<3)
							 |(drv3245_status.vgs_faults.bit.VGS_LA<<4)|(drv3245_status.vgs_faults.bit.VGS_HA<<5);

     DID_0101.Data_0101[6] = (uint8)(drv3245_status.hs_gate_control.bit.IDRIVEP_HS)|(drv3245_status.hs_gate_control.bit.IDRIVEN_HS<<2)
    						 |(drv3245_status.hs_gate_control.bit.TDRIVEN<<4);

     DID_0101.Data_0101[7] = (uint8)(drv3245_status.ls_gate_control.bit.IDRIVEP_LS)|(drv3245_status.ls_gate_control.bit.IDRIVEN_LS<<2)
    						 |(drv3245_status.ls_gate_control.bit.TDRIVEP<<4)|(drv3245_status.ls_gate_control.bit.IHOLD<<6);

     DID_0101.Data_0101[8] = (uint8)(drv3245_status.gate_control.all>>8U);


     DID_0101.Data_0101[9] = (uint8)(drv3245_status.gate_control.bit.TVDS)|(drv3245_status.gate_control.bit.TBLANK<<2)
     							 |(drv3245_status.gate_control.bit.DEAD_TIME<<4);


	 DID_0101.Data_0101[10] = (uint8)(drv3245_status.ic_operation.bit.CLR_FLTS)|(drv3245_status.ic_operation.bit.PVDD_OV_MODE<<1)
							 |(drv3245_status.ic_operation.bit.DIS_SNS_OCP<<2)|(drv3245_status.ic_operation.bit.DEADT_MD<<3)
							 |(drv3245_status.ic_operation.bit.STP_MODE<<5)|(drv3245_status.ic_operation.bit.VGS_MODE<<6);

	 DID_0101.Data_0101[11] = (uint8)(drv3245_status.shunt_amp_control.bit.GAIN_CS1)|(drv3245_status.shunt_amp_control.bit.GAIN_CS2<<2)
							 |(drv3245_status.shunt_amp_control.bit.GAIN_CS3<<4)|(drv3245_status.shunt_amp_control.bit.CSA2_DIAG<<6);

	 DID_0101.Data_0101[12] = (uint8)(drv3245_status.ic_control_0.bit.WARN_MODE)|(drv3245_status.ic_control_0.bit.CFG_CRC<<1)
							 |(drv3245_status.ic_control_0.bit.IDRIVESD<<2);

	 DID_0101.Data_0101[13] = (uint8)(drv3245_status.ic_control_1.bit.CFG_CRC_DIAG)|(drv3245_status.ic_control_1.bit.DRV_SH_DIAG<<1)
							 |(drv3245_status.ic_control_1.bit.DRVOFF_DIAG<<2);

	 DID_0101.Data_0101[14] = (uint8)(drv3245_status.phc_control.bit.PHC_COMP_EN)|(drv3245_status.phc_control.bit.PHC_OUTEN<<1)
							 |(drv3245_status.phc_control.bit.PHC_MODE<<2);

	 DID_0101.Data_0101[15] = (uint8)(drv3245_status.phc_status.bit.PHCACOMP)|(drv3245_status.phc_status.bit.PHCBCOMP<<1)
							 |(drv3245_status.phc_status.bit.PHCCCOMP<<2);

	 DID_0101.Data_0101[16] = (uint8)(drv3245_status.volt_reg_control.bit.CP_SD_MODE)|(drv3245_status.volt_reg_control.bit.CP_DIS<<1)
							 |(drv3245_status.volt_reg_control.bit.VCPHOVABS_MODE<<2)|(drv3245_status.volt_reg_control.bit.VREF_SCALE<<3);

	 DID_0101.Data_0101[17] = (uint8)(drv3245_status.vdc_sense_control_0.bit.VDS_MODE)|(drv3245_status.vdc_sense_control_0.bit.VDS_DIAG<<3)
							 |(drv3245_status.vdc_sense_control_0.bit.VDS_LVL<<4);

	 DID_0101.Data_0101[18] = (uint8)(drv3245_status.vdc_sense_control_1.bit.VDS_CFG_MODE)|(drv3245_status.vdc_sense_control_1.bit.VDS_LEVEL_HSA<<1);

	 DID_0101.Data_0101[19] = (uint8)(drv3245_status.vdc_sense_control_2.bit.VDS_LEVEL_HSC);

	 DID_0101.Data_0101[20] = (uint8)(drv3245_status.vgs_control.bit.VGS_BLANK)|(drv3245_status.vgs_control.bit.VGS_DEG<<2)
							 |(drv3245_status.vgs_control.bit.VGS_TH_MODE<<4);



	 /*********************** DID_102 *********************************/


	 DID_0102.Data_0102[0] = (uint8)(drv3245_status.spi_test_control.bit.SPI_TEST);

	 DID_0102.Data_0102[1] = (uint8)(drv3245_status.spiwr_crc_control.bit.SPIWR_CRC);

	 DID_0102.Data_0102[2] = (uint8)(drv3245_status.spird_crc_control.bit.SPIRD_CRC);


	 DID_0102.Data_0102[3] = (uint8)(drv3245_status.ic_status1.bit.CLK_MON_FAULT)|(drv3245_status.ic_status1.bit.CFG_CRC_FAULT<<1)
							 |(drv3245_status.ic_status1.bit.INT_REG_FAULT<<2)|(drv3245_status.ic_status1.bit.DEAD_FAULT<<3)
							 |(drv3245_status.ic_status1.bit.STP_FAULT<<4);

	 DID_0102.Data_0102[4] = (uint8)(drv3245_status.ic_status2.bit.DEV_MODE_FAULT)|(drv3245_status.ic_status2.bit.ABIST_FAULT<<1)
							 |(drv3245_status.ic_status2.bit.CLKMON_BIST_FAULT<<2)|(drv3245_status.ic_status2.bit.EE_CRC_FAULT<<3)
							 |(drv3245_status.ic_status2.bit.SPI_CLK_FAULT<<4)|(drv3245_status.ic_status2.bit.SPI_ADDR_FAULT<<5)
							 |(drv3245_status.ic_status2.bit.SPIWR_CRC_FAULT<<6);



	 DID_0102.Data_0102[5] = (drv3245_status.oc_vds_faults.bit.VDS_HA);

	 DID_0102.Data_0102[6] = (drv3245_status.ic_faults.bit.PVDD_UVLO);

	 /*continue of 101[9]*/
     DID_0102.Data_0102[7] = (uint8)(drv3245_status.gate_control.bit.PWM_MODE)
     							 |(drv3245_status.gate_control.bit.PWM_COM<<2)|(drv3245_status.gate_control.bit.ENABLE_DRV<<3);


     DID_0102.Data_0102[8] = (drv3245_status.vdc_sense_control_2.bit.VDS_LEVEL_HSB);

}

static void Set_DID_0103(void)

{
	uint16 test_data_0103 = 0;

	DID_0103.Data_0103[0]    = (uint8)(s16_PwmDuty>>8U);
	DID_0103.Data_0103[1]    = (uint8)(s16_PwmDuty);

	test_data_0103 =((uint16)(s32_CurrentShaftPostion_A*10));

	DID_0103.Data_0103[2]    = (uint8)(test_data_0103>>8U);
	DID_0103.Data_0103[3]    = (uint8)(test_data_0103);

	test_data_0103 = ((uint16)(s32_CurrentShaftPostion_B*10));

	DID_0103.Data_0103[4]    = (uint8)(test_data_0103>>8U);
	DID_0103.Data_0103[5]    = (uint8)(test_data_0103);

	test_data_0103 = ((uint16)(f32_Position_Real*10));

	DID_0103.Data_0103[6]    = (uint8)(test_data_0103>>8U);
	DID_0103.Data_0103[7]    = (uint8)(test_data_0103);

	test_data_0103 = ((uint16)(s32_CurrentShaftPostion_ramp*10));

	DID_0103.Data_0103[8]    = (uint8)(test_data_0103>>8U);
	DID_0103.Data_0103[9]    = (uint8)(test_data_0103);

	test_data_0103 = ((uint16)(ramp_resp_time_para*10));

	DID_0103.Data_0103[10]   = (uint8)(test_data_0103>>8U);
    DID_0103.Data_0103[11]   = (uint8)(test_data_0103);

    test_data_0103 = ((uint16)(Wr*10));

	DID_0103.Data_0103[12]   = (uint8)(test_data_0103>>8U);
	DID_0103.Data_0103[13]   = (uint8)(test_data_0103);

	DID_0103.Data_0103[14]   = (ui16_Hall_U_mcu)|(ui16_Hall_V_mcu<<1)|(ui16_Hall_W_mcu<<2)|(SCU_Fault_flag<<3)|
			                 (CAN_Fault_flag<<4)|(Pwr_Vehicle_Under_BAT<<5);

    DID_0103.Data_0103[15]   = (Pwr_Vehicle_Over_BAT)|(SCU_FAULT_State_Flag.EEPROM_Checksum_Error_flag<<1)|(SCU_FAULT_State_Flag.Pos_Sens_Signal_Short_flag<<2)|
    		                  (SCU_FAULT_State_Flag.Pos_Sens_Signal_Open_flag<<3)|(SCU_FAULT_State_Flag.Pos_Sens_Power_Open_flag<<4)|(SCU_FAULT_State_Flag.Pos_Sens_Power_Short_flag<<5)
							  |(SCU_FAULT_State_Flag.Pos_Sens_Data_Error_flag<<6)|(SCU_FAULT_State_Flag.Pos_Sens_Output_Error_flag<<7);

	DID_0103.Data_0103[16]   = (SCU_FAULT_State_Flag.Motor_Battery_Open_flag)|(SCU_FAULT_State_Flag.Motor_UVW_Short_flag<<1)|(SCU_FAULT_State_Flag.Motor_Over_Current_flag<<2)|
						     (SCU_FAULT_State_Flag.Motor_System_error_flag<<3)|(SCU_FAULT_State_Flag.Motor_Operate_error_flag<<4)|(SCU_FAULT_State_Flag.HallIC_Battery_Short_flag<<5)|
							 (SCU_FAULT_State_Flag.GateDriver_Error_flag<<6)|(SCU_FAULT_State_Flag.P_Pos_Output_Battery_Short_flag<<7);

	DID_0103.Data_0103[17]   = (SCU_FAULT_State_Flag.P_Pos_Output_Battery_Open_flag)|(SCU_FAULT_State_Flag.ACT_Init_Error_flag<<1)|(SCU_FAULT_State_Flag.Hall_IC_Open_flag<<2)|
							(SCU_FAULT_State_Flag.Motor_UVW_GND_Short_flag<<3)|(SCU_FAULT_State_Flag.ACT_Stuck_flag<<4)|(SCU_FAULT_State_Flag.HallIC_Power_Open_flag<<5)|
							(SCU_FAULT_State_Flag.HallIC_Power_Short_flag<<6)|(SCU_FAULT_State_Flag.P_Can_Bus_Off_flag<<7);

	DID_0103.Data_0103[18]   = (SCU_FAULT_State_Flag.P_CAN_EMS_Timeout_flag)|(SCU_FAULT_State_Flag.P_CAN_TCU_Timeout_flag<<1)|(SCU_FAULT_State_Flag.P_CAN_ESC_Timeout_flag<<2)|
							(SCU_FAULT_State_Flag.P_CAN_CGW_Timeout_flag<<3)|(SCU_FAULT_State_Flag.L_CAN_TCU_Timeout_flag<<4)|(SCU_FAULT_State_Flag.P_CAN_SBW_Timeout_flag<<5)
							|(SCU_FAULT_State_Flag.L_CAN_SBW_Timeout_flag<<6)|(SCU_FAULT_State_Flag.P_CAN_SBW_ERROR_flag<<7);

}

static void Set_DID_0104(void)
{

	uint16 test_data_0104 = 0;

	DID_0104.Data_0104[0]= (VBAT_STAT)|(IGN_STAT<<1)|(Ig_Off_detect_flag<<2)|(Fault_SBC<<3)|(can_tx_flag<<4)
						|(detect_Motor_Move_flag<<5)|(detect_Hall_IC_Move_flag<<6)|(detect_InhibitSw_Move_flag<<7);

	DID_0104.Data_0104[1]= (uint8)(button_status_change_detect)|(uint8)(edge_p_button_flag<<1)|(uint8)(edge_r_button_flag<<2)|
		 	 	 	 	 (uint8)(edge_n_button_flag<<3)|(uint8)(edge_d_button_flag<<4)|(uint8)(Ig_Off_Auto_P_flag<<5)
						 |(uint8)(door_open_detect<<6);

	DID_0104.Data_0104[2]= (SCU_EPB_Auto_Req_flag)|(Door_Open_Auto_P_flag<<1)|(trans_vehicle_speed_condition<<2)|(trans_DR_vehicle_speed_condition<<3)
						 |(trans_vehicle_rpm_condition<<4)|(scu_warn_msg_1_p_fault<<5)|(scu_warn_msg_2_not_condition<<6)
						|(scu_warn_msg_3_brake_condition<<7);

	DID_0104.Data_0104[3]= (scu_warn_msg_4_speed_condition)|(scu_warn_msg_5_p_pos_again<<1)|(scu_warn_msg_7_r_pos_again<<2)
						 |(scu_warn_msg_9_scu_fault<<3)|(scu_warn_msg_13_n_pos_again<<4)|(scu_warn_msg_14_d_pos_again<<5);

	DID_0104.Data_0104[4] = (uint8)(System_Power);
	DID_0104.Data_0104[5] = (uint8)(n_standby_mode);
	DID_0104.Data_0104[6] = (uint8)(sleep_time>>8U);
	DID_0104.Data_0104[7] = (uint8) (sleep_time);
	DID_0104.Data_0104[8] = (uint8)(Apl_SCU11_CAN.SCU_POS_ACT);
	DID_0104.Data_0104[9] = (uint8)(Apl_SCU11_CAN.SCU_POS_TAR);

	test_data_0104=((uint16)(Position_PID.kp*10));

	DID_0104.Data_0104[10]= (uint8)(test_data_0104>>8U);
	DID_0104.Data_0104[11]= (uint8)(test_data_0104);

	test_data_0104=((uint16)(Position_PID.ki*10));

	DID_0104.Data_0104[12]=(uint8)( test_data_0104>>8U);
	DID_0104.Data_0104[13]= (uint8)(test_data_0104);

	test_data_0104=((uint16)(Position_PID.kd*10));

	DID_0104.Data_0104[14]= (uint8)(test_data_0104>>8U);
	DID_0104.Data_0104[15]= (uint8)(test_data_0104);

	test_data_0104=((uint16)(Position_PID.Limit_max*10));

	DID_0104.Data_0104[16]= (uint8)(test_data_0104>>8U);
	DID_0104.Data_0104[17]= (uint8)(test_data_0104);


}

static void Set_DID_0105(void)
{


}

static void Set_DID_0106(void)
{

}

static void Set_F180(void)
{
	DID_F180[0]	=(tU08)'T';
	DID_F180[1]	=(tU08)'M';
	DID_F180[2]	=(tU08)'c';
	DID_F180[3]	=(tU08)'_';
	DID_F180[4]	=(tU08)'S';
	DID_F180[5]	=(tU08)'C';
	DID_F180[6]	=(tU08)'U';
	DID_F180[7]	=(tU08)'_';
	DID_F180[8]	=(tU08)'B';
	DID_F180[9]	=(tU08)'L';
	DID_F180[10]=(tU08)'_';
	DID_F180[11]=(tU08)'1';
	DID_F180[12]=(tU08)'.';
	DID_F180[13]=(tU08)'0';
	DID_F180[14]=(tU08)'0';
}

static void Set_F189(void)
{
	DID_F189[0] =(tU08)'T';
	DID_F189[1] =(tU08)'M';
	DID_F189[2] =(tU08)'c';
	DID_F189[3] =(tU08)'_';
	DID_F189[4] =(tU08)'S';
	DID_F189[5] =(tU08)'C';
	DID_F189[6] =(tU08)'U';
	DID_F189[7] =(tU08)'_';
	DID_F189[8] =(tU08)'0';
	DID_F189[9] =(tU08)'.';
	DID_F189[10] =(tU08)'0';
	DID_F189[11] =(tU08)'3';
	DID_F189[12] =(tU08)'_';
	DID_F189[13] =(tU08)'T';
	DID_F189[14] =(tU08)'S';
	DID_F189[15] =(tU08)'T';
}

static void Set_F192(void)
{
	DID_F192[0] =(tU08)'S';
	DID_F192[1] =(tU08)'C';
	DID_F192[2] =(tU08)'U';
	DID_F192[3] =(tU08)'_';
	DID_F192[4] =(tU08)'0';
	DID_F192[5] =(tU08)'.';
	DID_F192[6] =(tU08)'0';
	DID_F192[7] =(tU08)'3';
	DID_F192[8] =(tU08)'_';
	DID_F192[9] =(tU08)'T';
	DID_F192[10] =(tU08)'S';
	DID_F192[11] =(tU08)'T';
}

static void Set_F194(void)
{
	DID_F194[0] =(tU08)'S';
	DID_F194[1] =(tU08)'C';
	DID_F194[2] =(tU08)'U';
	DID_F194[3] =(tU08)'_';
	DID_F194[4] =(tU08)'0';
	DID_F194[5] =(tU08)'.';
	DID_F194[6] =(tU08)'0';
	DID_F194[7] =(tU08)'3';
	DID_F194[8] =(tU08)'_';
	DID_F194[9] =(tU08)'T';
	DID_F194[10] =(tU08)'S';
	DID_F194[11] =(tU08)'T';
}

static void Set_F195(void)
{
	DID_F195[0] =(tU08)'T';
	DID_F195[1] =(tU08)'M';
	DID_F195[2] =(tU08)'c';
	DID_F195[3] =(tU08)'_';
	DID_F195[4] =(tU08)'S';
	DID_F195[5] =(tU08)'C';
	DID_F195[6] =(tU08)'U';
	DID_F195[7] =(tU08)'_';
	DID_F195[8] =(tU08)'0';
	DID_F195[9] =(tU08)'.';
	DID_F195[10] =(tU08)'0';
	DID_F195[11] =(tU08)'3';
	DID_F195[12] =(tU08)'_';
	DID_F195[13] =(tU08)'T';
	DID_F195[14] =(tU08)'S';
	DID_F195[15] =(tU08)'T';
}



static void Set_F198(void)
{
	DID_F198[0] =(tU08)'H';
	DID_F198[1] =(tU08)'K';
	DID_F198[2] =(tU08)'M';
	DID_F198[3] =(tU08)'C';
	DID_F198[4] =(tU08)' ';
	DID_F198[5] =(tU08)' ';
	DID_F198[6] =(tU08)' ';
	DID_F198[7] =(tU08)' ';
	DID_F198[8] =(tU08)' ';
	DID_F198[9] =(tU08)' ';
}



static void Set_F19A(void)
{
	DID_F19A[0] =(tU08)'H';
	DID_F19A[1] =(tU08)'K';
	DID_F19A[2] =(tU08)'M';
	DID_F19A[3] =(tU08)'C';
	DID_F19A[4] =(tU08)' ';
	DID_F19A[5] =(tU08)' ';
	DID_F19A[6] =(tU08)' ';
	DID_F19A[7] =(tU08)' ';
	DID_F19A[8] =(tU08)' ';
	DID_F19A[9] =(tU08)' ';
}

static void Set_F1A0(void)
{
	DID_F1A0[0] =(tU08)'0';
	DID_F1A0[1] =(tU08)'.';
	DID_F1A0[2] =(tU08)'0';
	DID_F1A0[3] =(tU08)'3';
}

