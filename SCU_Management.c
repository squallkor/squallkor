/*************************************************************************************************
| Project Name:	SCU Actuator
|    File Name: SCU_Management.c
|    Compiler : Tasking Compiler
|  Description: SCU Management
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
| 2019-03-07  1.0           SDH     Initiate
**************************************************************************************************/

#include "include.h"

void SCU_Management(void)
{
	SCU_SBW_Signal_Detection();
	SCU_Shift_application();
	SCU_EPB_Control_Main();
	SCU_N_Wash_Mode_Management();
	SCU_Warn_Management();
	//SCU_Failsafe_Management();
	SCU_Indicator_application();
}
