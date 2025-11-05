/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:24:49
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 20:24:51
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Encoder.c
 * @Description: ABZ编码器“有感”测速
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Encoder.h"
#include "filter.h"

extern TIM_HandleTypeDef htim3;
static uint8_t Encoder_InitFlag = 0;
#define EncoderR_noise      8.0f    // R值，传感器噪声大则设大
#define EncoderQ_noise      0.0008f   // Q值，系统变化快则设大

static void Encoder_Init(void) {
    // 启动编码器接口模式
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    // 设置TIM3计数器初始值
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}

static uint16_t Encoder_GetCount(void) {
    return (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);
}


static void Encoder_ReadSpeed(float *real_speed) {
    if (!Encoder_InitFlag) {
        Encoder_Init();
        Encoder_InitFlag = 1;
    }
    static uint16_t Last_Count = 0;
    static uint32_t Last_Time = 0;
    uint16_t This_Count = Encoder_GetCount();
    uint32_t This_Time = HAL_GetTick();
    int32_t Encoder_num =  This_Count - Last_Count;
    int32_t Encoder_time = This_Time - Last_Time;
    if (Encoder_num >= 3000) {
        Encoder_num -= 4096;
    }
    if (Encoder_num <= -3000) {
        Encoder_num += 4096;
    }
    if (Encoder_time) {
        *real_speed = (Encoder_num*Sg_PI*2000.0f)/(Encoder_time*4096.0f);
    }
    Last_Count = This_Count;
    Last_Time = This_Time;
}


float Encoder_FilterSpeed(void) {
    static float data = 0;
    Encoder_ReadSpeed(&data);
    return kalman_filter_std(data, EncoderR_noise, EncoderQ_noise);
}

