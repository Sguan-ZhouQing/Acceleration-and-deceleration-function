/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-05 21:01:19
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-12 01:31:26
 * @FilePath: \demo_SguanFOCv2.0\Software\GlobalSet.c
 * @Description: 时间调度函数实现
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "GlobalSet.h"
// 外部函数包含
#include "Key.h"
#include "Encoder.h"
#include "led.h"
#include "foc.h"
#include "motor_pid.h"

extern PID_STRUCT SguanVal;

/**
 * @description: 全局时间调度函数Tick
 * @return {*}
 */
void GlobalSet_Tick(void) {
    // static uint8_t Set_OutTime = 0;
    // Set_OutTime++;
    // if (Set_OutTime % 10) {
    //     // your code
    // }
    // if (Set_OutTime == 100) {
    //     Set_OutTime = 0;
    // }
    Key_Tick();
    LED_Tick();
    Encoder_Tick();
    // PID_Control(&SguanVal);
}
