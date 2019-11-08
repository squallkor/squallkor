/*
 * Motor_Management.c
 *
 *  Created on: 2019. 2. 13.
 *      Author: dhshin
 */

#include "include.h"

#define HALL_IC_POS_P 10
#define HALL_IC_POS_R 51
#define HALL_IC_POS_N 70
#define HALL_IC_POS_D 105
#define HALL_IC_POS_L 125

#define PHC_CNTRL_EN()				drv_writeRegister(0x0A, 0x407)
#define MT_DRV_OP_ON() 				drv_writeRegister(0x07, 0x400)
#define MT_DRV_OP_OFF() 			drv_writeRegister(0x07, 0x000)
#define MT_VGS_MODE_DETECT_OFF()	drv_writeRegister(0x09, 0x100)

uint16 gate_driver_read_data = 0x00;
static uint8 gate_driver_write_data = 0x01;
float ramp_resp_time_para= 0.3;

uint8 INIT_POS_flag = 0u;
uint32 recovery_flag = 0u;
uint8 control_pi = 2u;

/*static void Fault_Detect_Function_OFF(void);*/
static void Motor_Position_Intialize_Funtcion(void);
static void Motor_State_Update_Group_1(void);
static void Motor_State_Update_Group_2(void);
static void Motor_Stat_Update_Fun_1(void);
static void Motor_Stat_Update_Fun_2(void);
static void Motor_Stat_Update_Fun_3(void);
static void Motor_Stat_Update_Fun_4(void);
static void Motor_Stat_Update_Fun_5(void);
static void Motor_Stat_Update_Fun_6(void);
/*static void Motor_stat_test(void);*/
/*static void Check_Init_Position(void);*/
static void Motor_Init_Pos_Check(void);


void Motor_50us_Main(void) {

	static uint8 current_control_cnt = 0;
	if(apl_init_complete_flag==1)
	{
		Init_Offset_Current_Detect();
		Making_Theta();
		if(current_control_cnt==1)
		{
			Current_Control_Main();
			current_control_cnt = 0u;
		}
		else{
			current_control_cnt++;
		}

		BLDC_Motor_Operation();
	}

}

float s32_CurrentShaftPostion_ramp = 0;
uint8 test_task_flag = 0;

void Motor_Manage_Main_1ms_task(void){
	/*test_task_flag = test_task_flag^1;*/
	/*P15_OUT.B.P0 = test_task_flag;*/
	/*P15_OUT.B.P0 = 1;*/

	GetShaftPositionSensor();	/*Hall IC Sensor*/
	Speed_Control_Main();

	if(drv3245_status.ic_status0.bit.SPI_OK==1U)
	{
		if(control_pi == 1)
		{

		}
		else if(control_pi == 2)
		{

		}
		else{
			s16_PwmDuty = 0u;
		}
	}

	if(s32_CurrentShaftPostion_ramp > s32_CurrentShaftPostion_Ref)
	{
		if((s32_CurrentShaftPostion_ramp - ramp_resp_time_para) <= s32_CurrentShaftPostion_Ref)
		{
			s32_CurrentShaftPostion_ramp = s32_CurrentShaftPostion_Ref;
		}
		else
		{
			s32_CurrentShaftPostion_ramp = s32_CurrentShaftPostion_ramp - ramp_resp_time_para;
		}
	}
	else if(s32_CurrentShaftPostion_ramp < s32_CurrentShaftPostion_Ref)
	{
		if((s32_CurrentShaftPostion_ramp + ramp_resp_time_para) >= s32_CurrentShaftPostion_Ref)
		{
			s32_CurrentShaftPostion_ramp = s32_CurrentShaftPostion_Ref;
		}
		else
		{
			s32_CurrentShaftPostion_ramp = s32_CurrentShaftPostion_ramp + ramp_resp_time_para;
		}
	}
	else
	{
		s32_CurrentShaftPostion_ramp = s32_CurrentShaftPostion_Ref;
	}


	/*Position_Control_Shaft();*/
	/*Position_Control_6Step();*/
	Apl_Motor_Error = P11_IN.B.P12;

	if((Apl_Motor_Error == 0) ||(drv3245_status.gate_control.bit.ENABLE_DRV == 0) )
	{
		drv_writeRegister(0x09, 0x3C2);
		drv_writeRegister(0x07, 0x436);
		recovery_flag++;
	}

}

uint8 sw_test = 0u;
uint16 sw_test_cnt = 0u;
void Motor_Manage_Main(void) {

	Motor_Position_Intialize_Funtcion();
	Making_Speed();

	if(sw_test==2u)
	{
		if(sw_test_cnt<200)
		{
			s32_CurrentShaftPostion_Ref = P_POSITION_DUTY;
		}
		else if(sw_test_cnt<400)
		{
			s32_CurrentShaftPostion_Ref = D_POSITION_DUTY;
		}
		else{
			sw_test_cnt = 0;
		}
		sw_test_cnt++;

	}

	if(sw_test ==3)
	{
		drv_writeRegister(0x05, 0x323);
		drv_writeRegister(0x06, 0x323);
		drv_writeRegister(0x09, 0x3C0);
		drv_writeRegister(0x0A, 0x005);
		drv_writeRegister(0x0C, 0x0D1);
		drv_writeRegister(0x07, 0x436);
		sw_test =0;
	}
	if(sw_test ==4)
	{
		drv_writeRegister(0x09, 0x3C2);
		drv_writeRegister(0x07, 0x436);

		sw_test =0;
	}
	if(sw_test ==5)
	{
		drv_writeRegister(0x09, 0x3C0);
	}
}


void SCU_ACT_GO_TO_GEAR_P(void){
	s32_CurrentShaftPostion_Ref = P_POSITION_DUTY; 	/* P Position*/
	f32_Position_Ref = 0;
}

void SCU_ACT_GO_TO_GEAR_R(void){
	s32_CurrentShaftPostion_Ref = R_POSITION_DUTY;	/* R Position*/
	f32_Position_Ref = 35;
}
void SCU_ACT_GO_TO_GEAR_N(void){
	s32_CurrentShaftPostion_Ref = N_POSITION_DUTY;	/* N Position*/
	f32_Position_Ref = 50;
}
void SCU_ACT_GO_TO_GEAR_D(void){
	s32_CurrentShaftPostion_Ref = D_POSITION_DUTY;  /* D Position*/
	f32_Position_Ref = 66;
}

/* Ver 0.02 Update for HW RESET 19.09.04 */
static void Motor_Init_Pos_Check(void){

	if(trans_vehicle_speed_condition==0)
	{
		/*Check_Init_Position();*/
	}
	else{
		s32_CurrentShaftPostion_Ref = P_POSITION_DUTY;

	}
}


void Fault_Detect_Function_OFF(void)
{

	/* PHC_CNTRL_EN(); */

	/* SHUNT AMP GAIN */
	/* drv_writeRegister(0x0A,0x00A); */

	/* vds_ctrl*/
	/* drv_writeRegister(0x0C,0x0D2); */

	/*  Gate Drive Control Register
		* ENABLE_DRV :  1b = ON
		* PWM_COM    :  0b = 1-PWM mode uses Synchronous rectification
		* DEAD TIME  :  0b = 150 nsec
		* TBLANK	 :  01b = 1.75 us
		* TVDS		 :  10b =3.5us VDS Sense deglitch time
	*/

	/* 6step contorl dont need dead time */
	/* drv_writeRegister(0x07,0x466); */


	/*  Gate Drive Control Register
	* ENABLE_DRV :  1b = ON
	* PWM_COM    :  0b = 1-PWM mode uses Synchronous rectification
	* DEAD TIME  : 100b = 895 nsec
	* TBLANK	 :  01b = 1.75 us
	* TVDS		 :  10b =3.5us VDS Sense deglitch time
	*/

	/* foc control must add dead time */
	/* drv_writeRegister(0x07,0x446); */

	drv_writeRegister(0x05, 0x323);
	drv_writeRegister(0x06, 0x323);

	/*  IC Operation Register (Address = 0x09) [reset = 0x00]
	 * 9-8 VGS_MODE   :  11b = VGS fault flags (VGS_xx) are set when a VGS fault is
	 *					 detected. Shutdown and nFAULT reporting are disabled
	 * 7 STP_MODE     :  1b = Shoot-through protection is enabled but no reporting is performed.
	 * 					 The gate driver outputs are forced low during a shoot-through condition. No SPI fault flag is set,
	 * 					 and the nFAULT pin stays high when the condition is detected.
	 * 6-5 DEADT_MODE :  10b = Dead-time protection is disabled. No dead time is inserted.
	 *                   No SPI fault flag is set and the nFAULT pin stays high.
	 * 4 DIS_SNS_OCP  :  01b = 1.75 us
	 * 3 RSVD  		  :  10b =3.5us VDS Sense deglitch time
	 * 2 PVDD_OV_MODE :
	 * 1 CLR_FLTS	  :
	 * 0 RSVD		  :
	 */

	drv_writeRegister(0x09, 0x3C0);
	drv_writeRegister(0x0A, 0x005);
	drv_writeRegister(0x0C, 0x0D1);
	drv_writeRegister(0x07, 0x436);
}


static void Motor_Position_Intialize_Funtcion(void)
{
	if(INIT_POS_flag == 0U)
	{
		if((inhibit_1_flag == 1U) && (inhibit_2_flag == 1U))
		{
			Motor_Init_Pos_Check();			/* Ver 0.02 Update for HW RESET 19.09.04 */
			s16_PwmDuty = 0;
			control_pi =2;
			INIT_POS_flag = 1U;
		}
		else
		{

		}
	}
	else
	{

	}
}


static void Motor_Stat_Update_Fun_1(void)
{

	if (gate_driver_write_data == 1) /* IC STATUS 0 */
	{
		drv3245_status.ic_status0.bit.OTW = gate_driver_read_data & 0x01;
		drv3245_status.ic_status0.bit.SPI_OK = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_status0.bit.BIST_STAT = (gate_driver_read_data & 0x04) >> 2;
		drv3245_status.ic_status0.bit.DRV_STAT = (gate_driver_read_data & 0x08) >> 3;
		drv3245_status.ic_status0.bit.VDS_STAT = (gate_driver_read_data & 0x20) >> 5;
		drv3245_status.ic_status0.bit.PVDD_OVFL = (gate_driver_read_data & 0x40) >> 6;
		drv3245_status.ic_status0.bit.PVDD_UVFL = (gate_driver_read_data & 0x80) >> 7;
		drv3245_status.ic_status0.bit.FAULT = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 2;
	} else if (gate_driver_write_data == 2) /* OV/VDS Faults 0 */
	{
		drv3245_status.oc_vds_faults.bit.SNS_A_OCP = gate_driver_read_data & 0x01;
		drv3245_status.oc_vds_faults.bit.SNS_B_OCP = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.oc_vds_faults.bit.SNS_C_OCP = (gate_driver_read_data & 0x04) >> 2;
		drv3245_status.oc_vds_faults.bit.VDS_LC = (gate_driver_read_data & 0x20) >> 5;
		drv3245_status.oc_vds_faults.bit.VDS_HC = (gate_driver_read_data & 0x40) >> 6;
		drv3245_status.oc_vds_faults.bit.VDS_LB = (gate_driver_read_data & 0x80) >> 7;
		drv3245_status.oc_vds_faults.bit.VDS_HB = (gate_driver_read_data & 0x100) >> 8;
		drv3245_status.oc_vds_faults.bit.VDS_LA = (gate_driver_read_data & 0x200) >> 9;
		drv3245_status.oc_vds_faults.bit.VDS_HA = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 3;
	} else if (gate_driver_write_data == 3) /* OV/VDS Faults 0 */
	{
		drv3245_status.ic_faults.bit.VCPH_OVLO_ABS = gate_driver_read_data & 0x01;
		drv3245_status.ic_faults.bit.VCPH_OVLO = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_faults.bit.VCPH_UVLO = (gate_driver_read_data & 0x04) >> 2;
		drv3245_status.ic_faults.bit.VCP_LSD_OVLO = (gate_driver_read_data & 0x08) >> 3;
		drv3245_status.ic_faults.bit.VCP_LSD_UVLO = (gate_driver_read_data & 0x10) >> 4;
		drv3245_status.ic_faults.bit.AVDD_UVLO = (gate_driver_read_data & 0x20) >> 5;
		drv3245_status.ic_faults.bit.AVDD_OVLO = (gate_driver_read_data & 0x40) >> 6;
		drv3245_status.ic_faults.bit.DVDD_OVLO = (gate_driver_read_data & 0x80) >> 7;
		drv3245_status.ic_faults.bit.RSVD = (gate_driver_read_data & 0x100) >> 8;
		drv3245_status.ic_faults.bit.PVDD_UVLO = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 4;
	}

	else if (gate_driver_write_data == 4) {
		drv3245_status.vgs_faults.bit.VGS_LC = (gate_driver_read_data & 0x20) >> 5;
		drv3245_status.vgs_faults.bit.VGS_HC = (gate_driver_read_data & 0x40) >> 6;
		drv3245_status.vgs_faults.bit.VGS_LB = (gate_driver_read_data & 0x80) >> 7;
		drv3245_status.vgs_faults.bit.VGS_HB = (gate_driver_read_data & 0x100) >> 8;
		drv3245_status.vgs_faults.bit.VGS_LA = (gate_driver_read_data & 0x200) >> 9;
		drv3245_status.vgs_faults.bit.VGS_HA = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 5;
	} else {

	}

}

static void Motor_Stat_Update_Fun_2(void)
{
	if (gate_driver_write_data == 5) {
		drv3245_status.hs_gate_control.bit.IDRIVEP_HS = gate_driver_read_data & 0x03;
		drv3245_status.hs_gate_control.bit.IDRIVEN_HS = (gate_driver_read_data & 0x30)>> 4;
		drv3245_status.hs_gate_control.bit.TDRIVEN = (gate_driver_read_data & 0x300) >> 8;
		gate_driver_write_data = 6;
	} else if (gate_driver_write_data == 6) {
		drv3245_status.ls_gate_control.bit.IDRIVEP_LS = gate_driver_read_data & 0x03;
		drv3245_status.ls_gate_control.bit.IDRIVEN_LS = (gate_driver_read_data & 0x30)
				>> 4;
		drv3245_status.ls_gate_control.bit.TDRIVEP = (gate_driver_read_data & 0x300) >> 8;
		drv3245_status.ls_gate_control.bit.IHOLD = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 7;
	} else if (gate_driver_write_data == 7) {
		drv3245_status.gate_control.bit.TVDS = gate_driver_read_data & 0x03;
		drv3245_status.gate_control.bit.TBLANK = (gate_driver_read_data & 0x0C) >> 2;
		drv3245_status.gate_control.bit.DEAD_TIME = (gate_driver_read_data & 0x70) >> 4;
		drv3245_status.gate_control.bit.PWM_MODE = (gate_driver_read_data & 0x180) >> 7;
		drv3245_status.gate_control.bit.PWM_COM = (gate_driver_read_data & 0x200) >> 9;
		drv3245_status.gate_control.bit.ENABLE_DRV = (gate_driver_read_data & 0x400) >> 10;

		gate_driver_write_data = 0x9;

	} else if (gate_driver_write_data == 0x9) {
		drv3245_status.ic_operation.bit.CLR_FLTS = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_operation.bit.PVDD_OV_MODE = (gate_driver_read_data & 0x04) >> 2;
		drv3245_status.ic_operation.bit.DIS_SNS_OCP = (gate_driver_read_data & 0x10) >> 4;
		drv3245_status.ic_operation.bit.DEADT_MD = (gate_driver_read_data & 0x60) >> 5;
		drv3245_status.ic_operation.bit.STP_MODE = (gate_driver_read_data & 0x80) >> 7;
		drv3245_status.ic_operation.bit.VGS_MODE = (gate_driver_read_data & 0x300) >> 8;


		gate_driver_write_data = 0xA;

	}
	else
	{

	}
}

static void Motor_Stat_Update_Fun_3(void)
{

	if (gate_driver_write_data == 0x0A) {

		drv3245_status.shunt_amp_control.bit.GAIN_CS1 = gate_driver_read_data & 0x03;
		drv3245_status.shunt_amp_control.bit.GAIN_CS2 = (gate_driver_read_data & 0x0C)>> 2;
		drv3245_status.shunt_amp_control.bit.GAIN_CS3 = (gate_driver_read_data & 0x30)>> 4;
		drv3245_status.shunt_amp_control.bit.CSA2_DIAG = (gate_driver_read_data & 0x40)>> 3;

		gate_driver_write_data = 0x1A;
	} else if (gate_driver_write_data == 0x1A) {
		drv3245_status.ic_control_0.bit.WARN_MODE = gate_driver_read_data & 0x01;
		drv3245_status.ic_control_0.bit.CFG_CRC = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_control_0.bit.IDRIVESD = (gate_driver_read_data & 0x0B) >> 2;

		gate_driver_write_data = 0x2A;

	} else if (gate_driver_write_data == 0x2A) {
		drv3245_status.ic_control_1.bit.CFG_CRC_DIAG = gate_driver_read_data & 0x01;
		drv3245_status.ic_control_1.bit.DRV_SH_DIAG = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_control_1.bit.DRVOFF_DIAG = (gate_driver_read_data & 0x04) >> 2;

		gate_driver_write_data = 0x4A;

	} else if (gate_driver_write_data == 0x4A) {
		drv3245_status.phc_control.bit.PHC_COMP_EN = gate_driver_read_data & 0x01;
		drv3245_status.phc_control.bit.PHC_OUTEN = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.phc_control.bit.PHC_MODE = (gate_driver_read_data & 0x04) >> 2;

		gate_driver_write_data = 0x5A;

	} else
	{

	}
}

static void Motor_Stat_Update_Fun_4(void)
{
	if (gate_driver_write_data == 0x5A) {
		drv3245_status.phc_status.bit.PHCACOMP = gate_driver_read_data & 0x01;
		drv3245_status.phc_status.bit.PHCBCOMP = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.phc_status.bit.PHCCCOMP = (gate_driver_read_data & 0x04) >> 2;

		gate_driver_write_data = 0xB;
	} else if (gate_driver_write_data == 0x0B) {
		drv3245_status.volt_reg_control.bit.CP_SD_MODE = gate_driver_read_data & 0x01;
		drv3245_status.volt_reg_control.bit.CP_DIS = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.volt_reg_control.bit.VCPHOVABS_MODE = (gate_driver_read_data & 0x04)>> 2;
		drv3245_status.volt_reg_control.bit.VREF_SCALE = (gate_driver_read_data & 0x200)	>> 8;

		gate_driver_write_data = 0xC;
	} else if (gate_driver_write_data == 0x0C) {
		drv3245_status.vdc_sense_control_0.bit.VDS_MODE = gate_driver_read_data & 0x07;
		drv3245_status.vdc_sense_control_0.bit.VDS_DIAG = (gate_driver_read_data & 0x08)
				>> 3;
		drv3245_status.vdc_sense_control_0.bit.VDS_LVL = (gate_driver_read_data & 0xF0)
				>> 4;

		gate_driver_write_data = 0x1C;
	} else if (gate_driver_write_data == 0x1C) {
		drv3245_status.vdc_sense_control_1.bit.VDS_CFG_MODE = gate_driver_read_data & 0x01;
		drv3245_status.vdc_sense_control_1.bit.VDS_LEVEL_HSA = (gate_driver_read_data
				& 0xF0) >> 4;

		gate_driver_write_data = 0x2C;
	}
	else
	{

	}

}

static void Motor_Stat_Update_Fun_5(void)
{
	if (gate_driver_write_data == 0x2C) {
		drv3245_status.vdc_sense_control_2.bit.VDS_LEVEL_HSC = gate_driver_read_data	& 0x0F;
		drv3245_status.vdc_sense_control_2.bit.VDS_LEVEL_HSB = (gate_driver_read_data & 0xF0) >> 4;

		gate_driver_write_data = 0x4C;
	} else if (gate_driver_write_data == 0x4C) {
		drv3245_status.vgs_control.bit.VGS_BLANK = gate_driver_read_data & 0x03;
		drv3245_status.vgs_control.bit.VGS_DEG = (gate_driver_read_data & 0x0B) >> 2;
		drv3245_status.vgs_control.bit.VGS_TH_MODE = (gate_driver_read_data & 0x10) >> 4;

		gate_driver_write_data = 0xD;
	} else if (gate_driver_write_data == 0x0D) {
		drv3245_status.spi_test_control.bit.SPI_TEST = gate_driver_read_data & 0xFF;
		gate_driver_write_data = 0x1D;

	} else if (gate_driver_write_data == 0x1D) {
		drv3245_status.spiwr_crc_control.bit.SPIWR_CRC = gate_driver_read_data & 0xFF;
		gate_driver_write_data = 0x2D;
	}
	else
	{

	}
}

static void Motor_Stat_Update_Fun_6(void)
{

	if (gate_driver_write_data == 0x2D) {
		drv3245_status.spird_crc_control.bit.SPIRD_CRC = gate_driver_read_data & 0xFF;

		gate_driver_write_data = 0x3D;
	} else if (gate_driver_write_data == 0x3D) {
		drv3245_status.ic_status1.bit.CLK_MON_FAULT = gate_driver_read_data & 0x01;
		drv3245_status.ic_status1.bit.CFG_CRC_FAULT = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_status1.bit.INT_REG_FAULT = (gate_driver_read_data & 0x04) >> 2;
		drv3245_status.ic_status1.bit.DEAD_FAULT = (gate_driver_read_data & 0x08) >> 3;
		drv3245_status.ic_status1.bit.STP_FAULT = (gate_driver_read_data & 0x10) >> 4;

		gate_driver_write_data = 0x4D;
	} else if (gate_driver_write_data == 0x4D) {
		drv3245_status.ic_status2.bit.DEV_MODE_FAULT = gate_driver_read_data & 0x01;
		drv3245_status.ic_status2.bit.ABIST_FAULT = (gate_driver_read_data & 0x02) >> 1;
		drv3245_status.ic_status2.bit.CLKMON_BIST_FAULT = (gate_driver_read_data & 0x04)
				>> 2;
		drv3245_status.ic_status2.bit.EE_CRC_FAULT = (gate_driver_read_data & 0x08) >> 3;
		drv3245_status.ic_status2.bit.SPI_CLK_FAULT = (gate_driver_read_data & 0x10) >> 4;
		drv3245_status.ic_status2.bit.SPI_ADDR_FAULT = (gate_driver_read_data & 0x20) >> 5;
		drv3245_status.ic_status2.bit.SPIWR_CRC_FAULT = (gate_driver_read_data & 0x40)
				>> 6;

		gate_driver_write_data = 0x1;

	}
	else{

	}
}

static void Motor_State_Update_Group_1(void)
{
	Motor_Stat_Update_Fun_1();
	Motor_Stat_Update_Fun_2();
	Motor_Stat_Update_Fun_3();

}

static void Motor_State_Update_Group_2(void)
{
	Motor_Stat_Update_Fun_4();
	Motor_Stat_Update_Fun_5();
	Motor_Stat_Update_Fun_6();
}

void Motor_Stat_Update(void)
{
	gate_driver_read_data = drv_readRegister(gate_driver_write_data);
	Motor_State_Update_Group_1();
	Motor_State_Update_Group_2();

}
