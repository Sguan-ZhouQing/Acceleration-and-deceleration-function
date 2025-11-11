/*
 * @Author: 星必尘Sguan
 * @Date: 2025-10-18 14:29:08
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-11 13:36:09
 * @FilePath: \demo_SguanFOCv2.0\Software\foc.c
 * @Description: FOC代码应用层书写；
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "foc.h"
// 外部函数包含
#include "Encoder.h"
#include "motor_SVPWM.h"
#include "fast_sin.h"
#include "motor_pid.h"

extern TIM_HandleTypeDef htim1;
uint8_t Motor_Dir = 0;
PID_STRUCT SguanVal;
extern float real_speed;

static float Set_du,Set_dv,Set_dw;

/**
 * @description: 初始化栅极驱动控制PWM
 * @return {*}
 */
void Sguan_FocInit(void) {
    // 开启SD使能栅极驱动器
    HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin,GPIO_PIN_SET);
    // 开启PWM输出
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    // d轴强拖，初始化对齐电角度
    float Init_du,Init_dv,Init_dw;
    SVPWM(0,0.2f,0,&Init_du,&Init_dv,&Init_dw);
    Set_Duty(Init_du,Init_dv,Init_dw);
    HAL_Delay(600);
    Encoder_Init();
    // 还原电机磁场
    SVPWM(0,0,0,&Init_du,&Init_dv,&Init_dw);
    Set_Duty(Init_du,Init_dv,Init_dw);
}

// 设置UWV电机通电
void Set_Duty(float d_u,float d_v,float d_w) {
    uint16_t Compare_u,Compare_v,Compare_w;
    Compare_u = (uint16_t)(4249 * (1.0f - d_u));
    Compare_v = (uint16_t)(4249 * (1.0f - d_v));
    Compare_w = (uint16_t)(4249 * (1.0f - d_w));
    if (Motor_Dir) {
        float temp = Compare_v;
        Compare_v = Compare_w;
        Compare_w = temp;
    }
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Compare_u);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Compare_v);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Compare_w);
}


// 简单位置环实现（仅电角度）
void SguanFOC_Position_OpenLoop(float target_angle,float Ud_Set) {
    target_angle = normalize_angle(target_angle);
    float Set_du,Set_dv,Set_dw;
    SVPWM(target_angle,Ud_Set,0,&Set_du,&Set_dv,&Set_dw);
    Set_Duty(Set_du,Set_dv,Set_dw);
}

// 简单速度环实现（速度开环）
void SguanFOC_Velocity_OpenLoop(float phi,float Uq_Set) {
    float Set_du,Set_dv,Set_dw;
    phi = phi*7.0f;
    phi = normalize_angle(phi);
    SVPWM(phi,0,Uq_Set,&Set_du,&Set_dv,&Set_dw);
    Set_Duty(Set_du,Set_dv,Set_dw);
}


void Speed_PidInit(void) {
    SguanVal.Kp = 0.00014f;
    SguanVal.Ki = 0.00000072f;
    SguanVal.Kd = 0.0f;
    SguanVal.OutMax = 0.9f;
    SguanVal.OutMin = -0.9f;
}

// 速度闭环实现（PID算法）
void SguanFOC_Velocity_CloseLoop(float phi) {
    // float Set_du,Set_dv,Set_dw;
    phi = phi*7.0f;
    phi = normalize_angle(phi);
    SVPWM(phi,0,SguanVal.Out,&Set_du,&Set_dv,&Set_dw);
    Set_Duty(Set_du,Set_dv,Set_dw);
}


void SguanFOC_Current_CloseLoop(float phi,float target_cur) {
    float Set_du,Set_dv,Set_dw;
    SVPWM(phi,0,SguanVal.Out,&Set_du,&Set_dv,&Set_dw);
    Set_Duty(Set_du,Set_dv,Set_dw);
}

