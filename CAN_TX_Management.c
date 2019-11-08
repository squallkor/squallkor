/*
 * CAN_Management.c
 *
 *  Created on: 2019. 3. 7.
 *      Author: dhshin
 */
#include "include.h"



CGW1_CAN Apl_CGW1_CAN;
CGW2_CAN Apl_CGW2_CAN;
CLU22_CAN Apl_CLU22_CAN;
EMS_H12_CAN Apl_EMS_H12_CAN;
EMS11_CAN Apl_EMS11_CAN;
EMS12_CAN Apl_EMS12_CAN;
EPB11_CAN Apl_EPB11_CAN;
ESP11_CAN Apl_ESP11_CAN;
ESP12_CAN Apl_ESP12_CAN;
LVR12_CAN Apl_LVR12_CAN;
SCU11_CAN Apl_SCU11_CAN;
TCU11_CAN Apl_TCU11_CAN;
TCU13_CAN Apl_TCU13_CAN;
WHL_PUL11_CAN Apl_WHL_PUL11_CAN;
WHL_SPD11_CAN Apl_WHL_SPD11_CAN;

uint8 lvr_crc_error_flag = 0u;
uint8 lvr_ac_error_flag = 0u;

static uint8 CRC_Output[7];

static void CAN_Output(void);
static void Set_SCU_CAN(void);
static void Set_CAN_SCU_POS_ACT(uint8 parameter);
static void Set_CAN_SCU_POS_TAR(uint8 parameter);
static void Set_CAN_SCU_ERR(uint8 parameter);
static void Set_CAN_SCU_EPB_REQ(uint8 parameter);
static void Set_CAN_SCU_WarnMsg(uint8 parameter);
static void Set_CAN_SCU_N_MODE(uint8 parameter);
static void Set_CAN_SCU_NCON_REQ(uint8 parameter);
static void Set_CAN_SCU_Alive1(uint8 parameter);
static void Set_CAN_SCU_ChkSum1(uint8 parameter);
/*static uint8 Calculate_CRC(const uint8 buf[]);*/
static uint8 Generate_Alive_Count(void);
static void Set_CRC_SCU_Data(void);

void CAN_MAIN_OUTPUT(void)
{
	CAN_Output();
}


void CAN_IL_TX_STOP(void)
{

	IlTxStop(0);
	IlTxStop(1);
}

void CAN_IL_TX_START(void)
{
	IlTxStart(0);
	IlTxStart(1);
}


static void CAN_Output(void){

	Set_SCU_CAN();
}


static void Set_SCU_CAN(void){

	Set_CAN_SCU_POS_ACT(Apl_SCU11_CAN.SCU_POS_ACT);
	Set_CAN_SCU_POS_TAR(Apl_SCU11_CAN.SCU_POS_TAR);
	Set_CAN_SCU_ERR(Apl_SCU11_CAN.SCU_ERR);
	Set_CAN_SCU_EPB_REQ(Apl_SCU11_CAN.SCU_EPB_REQ);
	Set_CAN_SCU_WarnMsg(Apl_SCU11_CAN.SCU_WarnMsg);
	Set_CAN_SCU_N_MODE(Apl_SCU11_CAN.SCU_N_MODE);
	Set_CAN_SCU_NCON_REQ(Apl_SCU11_CAN.SCU_NCON_REQ);

	Apl_SCU11_CAN.SCU_Alive1 = Generate_Alive_Count();
	Set_CAN_SCU_Alive1(Apl_SCU11_CAN.SCU_Alive1);

	Set_CRC_SCU_Data();
	Apl_SCU11_CAN.SCU_ChkSum1 = Calculate_CRC(CRC_Output);
	Set_CAN_SCU_ChkSum1(Apl_SCU11_CAN.SCU_ChkSum1);

}
static void Set_CAN_SCU_POS_ACT(uint8 parameter)
{
	Can_WriteSig_SCU_POS_ACT_0(parameter);
	Can_WriteSig_SCU_POS_ACT_1(parameter);
}

static void Set_CAN_SCU_POS_TAR(uint8 parameter)
{
	Can_WriteSig_SCU_POS_TAR_0(parameter);
	Can_WriteSig_SCU_POS_TAR_1(parameter);
}

static void Set_CAN_SCU_ERR(uint8 parameter)
{
	Can_WriteSig_SCU_ERR_0(parameter);
	Can_WriteSig_SCU_ERR_1(parameter);
}

static void Set_CAN_SCU_EPB_REQ(uint8 parameter)
{
	Can_WriteSig_SCU_EPB_REQ_0(parameter);
	Can_WriteSig_SCU_EPB_REQ_1(parameter);
}

static void Set_CAN_SCU_WarnMsg(uint8 parameter)
{
	Can_WriteSig_SCU_WarnMsg_0(parameter);
	Can_WriteSig_SCU_WarnMsg_1(parameter);
}

static void Set_CAN_SCU_N_MODE(uint8 parameter)
{
	Can_WriteSig_SCU_N_MODE_0(parameter);
	Can_WriteSig_SCU_N_MODE_1(parameter);
}

static void Set_CAN_SCU_NCON_REQ(uint8 parameter)
{
	Can_WriteSig_SCU_NCON_REQ_0(parameter);
	Can_WriteSig_SCU_NCON_REQ_1(parameter);
}

static void Set_CAN_SCU_Alive1(uint8 parameter)
{
	Can_WriteSig_SCU_Alive1_0(parameter);
	Can_WriteSig_SCU_Alive1_1(parameter);
}

static void Set_CAN_SCU_ChkSum1(uint8 parameter)
{
	Can_WriteSig_SCU_ChkSum1_0(parameter);
	Can_WriteSig_SCU_ChkSum1_1(parameter);
}


/********************************************************************************************
| Project Name: TMc SCU
| File Name: CAN_Management.c
| Func Name: Calculate_CRC
| Func Type: static void
| Return Type : uint8
| DESCRIPTION: Calculate CRC
|
| Output: return CRC
| Output Type : return uint8
| 0x0 ~ 0xFF : Calculated Data
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2019-05-08  1.00     dhshin    Creation
|********************************************************************************************/


uint8 Calculate_CRC(const uint8 buf[])
{
	 const uint8 Crc_Table[256]={
	 0x00U,	0x1DU,	0x3AU,	0x27U,	0x74U,	0x69U,	0x4EU,	0x53U,
	 0xE8U,	0xF5U,	0xD2U,	0xCFU,	0x9CU,	0x81U,	0xA6U,	0xBBU,
	 0xCDU,	0xD0U,	0xf7U,	0xEAU,	0xB9U,	0xA4U,	0x83U,	0x9EU,
	 0x25U,	0x38U,	0x1FU,	0x2U,	0x51U,	0x4CU,	0x6BU,	0x76U,
	 0x87U,	0x9AU,	0xBDU,	0xA0U,	0xF3U,	0xEEU,	0xC9U,	0xD4U,
	 0x6FU,	0x72U,	0x55U,	0x48U,	0x1BU,	0x6U,	0x21U,	0x3CU,
	 0x4AU,	0x57U,	0x70U,	0x6DU,	0x3EU,	0x23U,	0x4U,	0x19U,
	 0xA2U,	0xBFU,	0x98U,	0x85U,	0xD6U,	0xCBU,	0xECU,	0xF1U,
	 0x13U,	0x0EU,	0x29U,	0x34U,	0x67U,	0x7AU,	0x5DU,	0x40U,
	 0xFBU,	0xE6U,	0xC1U,	0xDCU,	0x8FU,	0x92U,	0xB5U,	0xA8U,
	 0xDEU,	0xC3U,	0xE4U,	0xF9U,	0xAAU,	0xB7U,	0x90U,	0x8DU,
	 0x36U,	0x2BU,	0x0CU,	0x11U,	0x42U,	0x5FU,	0x78U,	0x65U,
	 0x94U,	0x89U,	0xAEU,	0xB3U,	0xE0U,	0xFDU,	0xDAU,	0xC7U,
	 0x7CU,	0x61U,	0x46U,	0x5BU,	0x8U,	0x15U,	0x32U,	0x2FU,
	 0x59U,	0x44U,	0x63U,	0x7EU,	0x2DU,	0x30U,	0x17U,	0x0AU,
	 0xB1U,	0xACU,	0x8BU,	0x96U,	0xC5U,	0xD8U,	0xFFU,	0xE2U,
	 0x26U,	0x3BU,	0x1CU,	0x1U,	0x52U,	0x4FU,	0x68U,	0x75U,
	 0xCEU,	0xD3U,	0xF4U,	0xE9U,	0xBAU,	0xA7U,	0x80U,	0x9DU,
	 0xEBU,	0xF6U,	0xD1U,	0xCCU,	0x9FU,	0x82U,	0xA5U,	0xB8U,
	 0x3U,	0x1EU,	0x39U,	0x24U,	0x77U,	0x6AU,	0x4DU,	0x50U,
	 0xA1U,	0xBCU,	0x9BU,	0x86U,	0xD5U,	0xC8U,	0xEFU,	0xF2U,
	 0x49U,	0x54U,	0x73U,	0x6EU,	0x3DU,	0x20U,	0x7U,	0x1AU,
	 0x6CU,	0x71U,	0x56U,	0x4BU,	0x18U,	0x5U,	0x22U,	0x3FU,
	 0x84U,	0x99U,	0xBEU,	0xA3U,	0xF0U,	0xEDU,	0xCAU,	0xD7U,
	 0x35U,	0x28U,	0x0FU,	0x12U,	0x41U,	0x5CU,	0x7BU,	0x66U,
	 0xDDU,	0xC0U,	0xE7U,	0xFAU,	0xA9U,	0xB4U,	0x93U,	0x8EU,
	 0xF8U,	0xE5U,	0xC2U,	0xDFU,	0x8CU,	0x91U,	0xB6U,	0xABU,
	 0x10U,	0x0DU,	0x2AU,	0x37U,	0x64U,	0x79U,	0x5EU,	0x43U,
	 0xB2U,	0xAFU,	0x88U,	0x95U,	0xC6U,	0xDBU,	0xFCU,	0xE1U,
	 0x5AU,	0x47U,	0x60U,	0x7DU,	0x2EU,	0x33U,	0x14U,	0x9U,
	 0x7FU,	0x62U,	0x45U,	0x58U,	0x0BU,	0x16U,	0x31U,	0x2CU,
	 0x97U,	0x8AU,	0xADU,	0xB0U,	0xE3U,	0xFEU,	0xD9U,	0xC4U,
	 };

	uint8 crc = 0xFFU;

	crc = Crc_Table[buf[0]^crc];
	crc = Crc_Table[buf[1]^crc];
	crc = Crc_Table[buf[2]^crc];
	crc = Crc_Table[buf[3]^crc];
	crc = Crc_Table[buf[4]^crc];
	crc = Crc_Table[buf[5]^crc];
	crc = Crc_Table[buf[6]^crc];
	crc = ~crc;

	return crc;
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

static void Set_CRC_SCU_Data(void)
{
	CRC_Output[0] = (uint8)((uint8)(Apl_SCU11_CAN.SCU_POS_ACT) +
							(uint8)(Apl_SCU11_CAN.SCU_ERR <<4U) +
							(uint8)(Apl_SCU11_CAN.SCU_EPB_REQ <<2U));
	CRC_Output[1] = (uint8)((uint8)(Apl_SCU11_CAN.SCU_WarnMsg) +
							(uint8)(Apl_SCU11_CAN.SCU_POS_TAR<<4));
	CRC_Output[2] = (uint8)((uint8)(Apl_SCU11_CAN.SCU_N_MODE) +
					    (uint8)(Apl_SCU11_CAN.SCU_NCON_REQ<<2));
	CRC_Output[3] = 0x0u;
	CRC_Output[4] = 0x0u;
	CRC_Output[5] = 0x0u;
	CRC_Output[6] = (uint8)(Apl_SCU11_CAN.SCU_Alive1);

}

/********************************************************************************************
| Project Name: TMc SCU
| File Name: CAN_Management.c
| Func Name: Genarate_Alive_Count
| Func Type: static void
| Return Type : uint8
| DESCRIPTION: Generate Alive Count Value
|
| Output: return CNT_Alive
| Output Type : return uint8
| 0x0 --> 0xF : Counting Value
|
| Date        Ver      Author  Description
| ----------  ---      ------  --------------------------------------------------------------
| 2017-06-16  1.00     hwkim    Creation
|********************************************************************************************/

static uint8 Generate_Alive_Count(void)
{
	static uint8 CNT_Alive;

	if(CNT_Alive >= 15U)
	{
		CNT_Alive = 0U;
	}
	else
	{
		CNT_Alive++;
	}

	return CNT_Alive;
}


