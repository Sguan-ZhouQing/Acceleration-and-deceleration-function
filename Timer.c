/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-08 19:26:48
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 21:16:35
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Timer.c
 * @Description: TIM定时中断统一管理函数;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Timer.h"
// 外部函数包含
#include "GlobalSet.h"

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
    }
}

