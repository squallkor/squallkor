/*
 * Current_Sensing_Management.c
 *
 *  Created on: 2019. 11. 6.
 *      Author: dhshin
 */


#include "include.h"

uint16 adc_CS_3 = 0;

float32 Current_Ref = 0;

float Current_U = 0;
float Current_V = 0;
float Current_W = 0;
uint32 Now_Current = 0;
float32 Now_Current_CAL = 0;

float32 f32_Tau = 0.0005;
float32 f32_Ts = 50e-6;
float32 f32_Current_U_Low = 0;
float32 f32_Current_U = 0;
float32 f32_Current_V_Low = 0;
float32 f32_Current_V = 0;
float32 f32_Current_W_Low = 0;
float32 f32_Current_W = 0;
float32 f32_U_Absolute = 0;
float32 f32_V_Absolute = 0;
float32 f32_W_Absolute = 0;
float32 f32_sum_current = 0;
float u_ratio = 0.0166;
float v_ratio = 0.017892377;
float w_ratio = 1.07;
uint32 Current_offset_ADC_CS1 = 0;
uint32 Current_offset_ADC_CS2 = 0;
uint8 init_current_offset = 0u;

/* ���� ������ ���ϴ� �Լ� ADC �ε����� ���� 1024 �����ؼ� ��հ��� ���������� �Ѵ�. dhshin 19.11.04*/
void Init_Offset_Current_Detect(void)
{
	static uint16 Current_offset_cnt = 0u;

	if(init_current_offset == 0)
	{
		if(Current_offset_cnt >1023)		/* 1024 �� �ؾ� ������ ����Ǿ� �����ϰ� �� */
		{
			Current_offset_ADC_CS1 = (Current_offset_ADC_CS1>>10);
			Current_offset_ADC_CS2 = (Current_offset_ADC_CS2>>10);
			init_current_offset = 1u;
		}
		else{
			Current_offset_ADC_CS1 = (Current_offset_ADC_CS1 + AI.CS_1);
			Current_offset_ADC_CS2 = (Current_offset_ADC_CS2 + AI.CS_2);
			Current_offset_cnt++;
		}
	}
	else{

	}

}


void Current_Sensing_U(void)
{
	if(init_current_offset==1)
	{
		Current_U = (float) (AI.CS_1);
		f32_Current_U = -((Current_U - Current_offset_ADC_CS1) * u_ratio);

	}

}

void Current_Sensing_V(void)
{
	if(init_current_offset==1)
	{
		Current_V = (float)(AI.CS_2);

		f32_Current_V = -((Current_V - Current_offset_ADC_CS2) * v_ratio);

		f32_Current_W = (-(f32_Current_U)-(f32_Current_V)) * w_ratio;

	}

}
