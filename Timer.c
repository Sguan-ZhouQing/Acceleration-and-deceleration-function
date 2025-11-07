/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-08 19:26:48
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-07 21:26:00
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Timer.c
 * @Description: TIM定时中断统一管理函数;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Timer.h"
// 外部函数包含
#include "GlobalSet.h"
#include "Current.h"
#include "led.h"

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
    if (htim == &htim1) {  // 1kHz中断
        // FOC_LoopHandler();
    }
    if (htim == &htim2) {  // 1kHz中断
        GlobalSet_Tick();
        ADC_Start_Conversion_Sequence();
        LED_Tick();

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
