/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:24:49
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-14 01:10:19
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

extern PID_STRUCT SguanPos;
extern PID_STRUCT SguanVal;
extern PID_STRUCT SguanCur;
extern float real_speed;

float temp_IA;
float temp_IC;
float my_alpha;
float my_beta;
float my_id;
float my_iq;
float mysin;
float mycos;
float target_speed;

float temp_IB;
float filtered_value_B;
float filtered_value_A;
float filtered_value_C;

extern float Adjustable_Data;

// ADC原始采样值数组
volatile uint32_t ADC_InjectedValues[4];
float radtemp;

// 获取DQ轴电流值
void Current_GetDQ(float *id, float *iq) {
    float sine_own,cosine_own,raw_data_id,raw_data_iq;
    float i_a = (ADC_InjectedValues[1] - 1905.7f)*10.0f*3.3f/(4096*0.005f);
    float i_c = (ADC_InjectedValues[2] - 1905.1f)*10.0f*3.3f/(4096*0.005f);
    // filtered_value_A = kalman_filter_dir_on(i_a,20.0f,0.1f);
    // filtered_value_C = kalman_filter_dir_off(i_c,20.0f,0.1f);
    filtered_value_A = i_a;
    filtered_value_C = i_c;
    fast_sin_cos(radtemp,&sine_own,&cosine_own);
    clarke(&my_alpha,&my_beta,i_a,i_c);
    park_corrected(&raw_data_id,&raw_data_iq,my_alpha,my_beta,sine_own,cosine_own);
    static float filtered_value_D;
    static float filtered_value_Q;
    filtered_value_D = low_pass_filter(raw_data_id, filtered_value_D, 0.005f);
    filtered_value_Q = low_pass_filter(raw_data_iq, filtered_value_Q, 0.005f);
    *id = filtered_value_D;
    *iq = filtered_value_Q;
}



void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{		
	ADC_InjectedValues[0] = ADC2->JDR1;           //母线电压
	ADC_InjectedValues[1] = ADC2->JDR2;           //获取A相电流
	ADC_InjectedValues[2] = ADC2->JDR3;          //获取C相电流
 	ADC_InjectedValues[3] = ADC2->JDR4;//使用波轮电位器给电机目标转速

    
    //  float pos_num = Encoder_GetPos();
    //  SguanPos.Ref = Adjustable_Data;
    //  SguanPos.Fbk = pos_num;
    //  PID_Control(&SguanPos);
    //  float rad_num = Encoder_GetRad();
    //  radtemp = normalize_angle(rad_num*7.0f);
    //  SguanFOC_Position_CloseLoop(radtemp);
     
     
     float rad_num = Encoder_GetRad();
     radtemp = normalize_angle(rad_num*7.0f);
    //  SguanFOC_Velocity_CloseLoop(radtemp);
    // float Set_du,Set_dv,Set_dw;
    SVPWM(radtemp,0,SguanPos.Out,&Set_du,&Set_dv,&Set_dw);
    Set_Duty(Set_du,Set_dv,Set_dw);
     
     // SguanCur.Ref = Adjustable_Data;
    // SguanCur.Fbk = my_iq;
    // PID_Control(&SguanCur);
    // float rad_num = Encoder_GetRad();
    // radtemp = normalize_angle(rad_num*7.0f);
    // SguanFOC_Current_CloseLoop(radtemp);


    // float pos_num = Encoder_GetPos();
    // SguanPos.Ref = Adjustable_Data;
    // SguanPos.Fbk = pos_num;
    // PID_Control(&SguanPos);
    // float rad_num = Encoder_GetRad();
    // radtemp = normalize_angle(rad_num*7.0f);
    // SguanFOC_Position_CloseLoop(radtemp);
}
