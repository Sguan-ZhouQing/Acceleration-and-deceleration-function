/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-08 19:26:48
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-10 15:13:02
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Timer.c
 * @Description: TIM定时中断统一管理函数;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Timer.h"
// 外部函数包含
#include "GlobalSet.h"
#include "Current.h"
#include "Encoder.h"
#include "foc.h"
#include "filter.h"
#include "motor_pid.h"

extern ADC_HandleTypeDef hadc2;
extern float real_speed;
extern float target_speed;
extern PID_STRUCT SguanVal;

/**
 * @description: 初始化1ms中断函数的时钟
 * @return {*}
 */
void Timer_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @description: TIM2中断回调函数，1ms的定时器定时中断;
 * @param {TIM_HandleTypeDef*} htim
 * @return {*}
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim1) {  // 10kHz中断
        HAL_ADCEx_InjectedStart_IT(&hadc2);
    }
    if (htim == &htim2) {  // 1kHz中断
        SguanVal.Ref = target_speed;
        GlobalSet_Tick();
    }
    if (htim == &htim4) {  // 5kHz中断
        // PID_Control(&SguanVal);
        // SguanFOC_Velocity_CloseLoop(Encoder_GetRad());
    }
}

// // ADC采样中断回调函数
// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
//     // 现在这个中断已经是每3个PWM周期触发一次了
//         ADC_Start_Conversion_Sequence();
//         LED_Tick();
//     }
// }
