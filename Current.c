/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:24:49
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-10 19:56:12
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Current.c
 * @Description: 电流采样实现
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Current.h"
// 外部变量包含
extern ADC_HandleTypeDef hadc2;
#include "foc.h"
#include "filter.h"
#include "Encoder.h"
#include "motor_SVPWM.h"
#include "fast_sin.h"
#include "motor_pid.h"

extern PID_STRUCT SguanVal;
extern float real_speed;

int32_t temp_IA;
int32_t temp_IC;
float my_alpha;
float my_beta;
float my_id;
float my_iq;
float mysin;
float mycos;
float target_speed;

extern float Adjustable_Data;

// ADC原始采样值数组
volatile uint32_t ADC_InjectedValues[4];
volatile uint32_t ADC_RegularValues[4];

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{		
	ADC_InjectedValues[0] = ADC2->JDR1;           //母线电压
	ADC_InjectedValues[1] = ADC2->JDR2;           //获取A相电流
	ADC_InjectedValues[2] = ADC2->JDR3;          //获取C相电流
 	ADC_InjectedValues[3] = ADC2->JDR4;//使用波轮电位器给电机目标转速
    static float filtered_value = 0.0f;
    filtered_value = low_pass_filter(((ADC2->JDR4)*3.3f*75.0f)/4095.0f, filtered_value, 0.003f);
    temp_IA = 1905 - ADC_InjectedValues[1];
    temp_IC =1905 - ADC_InjectedValues[2];
    static float filtered_value_A = 0.0f;
    filtered_value_A = low_pass_filter(temp_IA, filtered_value_A, 0.008f);
    static float filtered_value_C = 0.0f;
    filtered_value_C = low_pass_filter(temp_IC, filtered_value_C, 0.008f);
    clarke_ac(&my_alpha,&my_beta,filtered_value_A,filtered_value_C);
    float rad_num = Encoder_GetRad();
    float radtemp = normalize_angle(rad_num*7.0f);
    fast_sin_cos(radtemp,&mysin,&mycos);
    park(&my_id,&my_iq,my_alpha,my_beta,mysin,mycos);
    target_speed = filtered_value;
    SguanVal.Fbk = real_speed;
    PID_Control(&SguanVal);
    SguanFOC_Velocity_CloseLoop(rad_num);
    // SguanFOC_Velocity_CloseLoop(rad_num,filtered_value);
}
