/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-14 09:31:21
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-15 20:45:27
 * @FilePath: \demo_SguanFOCv2.0\SguanFOC\SguanUser_Data.h
 * @Description: SguanFOC的“用户接口”库
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#ifndef __SGUANUSER_DATA_H
#define __SGUANUSER_DATA_H
/* USER CODE BEGIN Includes */
#include "SguanFOC.h"

/* USER CODE END Includes */

/**
 * @description: 宏定义启动电机ABCD的使用(重要)
 * @key_reminder: 请依据需求开启“电机定义”(重要)
 */
#define SguanMotor0
// #define SguanMotor1
// #define SguanMotor2
// #define SguanMotor3

void Sguan_ParameterSet(void) {
    #ifdef SguanMotor0
    Sguan0.Foc.Period = 4249;

    Sguan0.Position.Kp = 0.15f;
    Sguan0.Position.Ki = 0.00000025f;
    Sguan0.Position.Kd = 0.0f;
    Sguan0.Position.OutMax = 0.35f;
    Sguan0.Position.OutMin = -0.35f;
    Sguan0.Position.D_Filter = 0.2f;
    Sguan0.Position.ErrLimltFlag = 0;
    Sguan0.Position.ErrMax = 0.0f;
    Sguan0.Position.ErrMin = 0.0f;
    
    Sguan0.Speed.Kp = 0.02226f;
    Sguan0.Speed.Ki = 0.000115f;
    Sguan0.Speed.Kd = 0.0f;
    Sguan0.Speed.OutMax = 0.9f;
    Sguan0.Speed.OutMin = -0.9f;
    Sguan0.Speed.D_Filter = 0.2f;
    Sguan0.Speed.ErrLimltFlag = 0;
    Sguan0.Speed.ErrMax = 0.0f;
    Sguan0.Speed.ErrMin = 0.0f;

    Sguan0.Id.Kp = 0.02226f;
    Sguan0.Id.Ki = 0.000115f;
    Sguan0.Id.Kd = 0.0f;
    Sguan0.Id.OutMax = 0.9f;
    Sguan0.Id.OutMin = -0.9f;
    Sguan0.Id.D_Filter = 0.2f;
    Sguan0.Id.ErrLimltFlag = 0;
    Sguan0.Id.ErrMax = 0.0f;
    Sguan0.Id.ErrMin = 0.0f;

    Sguan0.Id.Kp = 0.02226f;
    Sguan0.Id.Ki = 0.000115f;
    Sguan0.Id.Kd = 0.0f;
    Sguan0.Id.OutMax = 0.9f;
    Sguan0.Id.OutMin = -0.9f;
    Sguan0.Id.D_Filter = 0.2f;
    Sguan0.Id.ErrLimltFlag = 0;
    Sguan0.Id.ErrMax = 0.0f;
    Sguan0.Id.ErrMin = 0.0f;
    #endif // SguanMotor0
    #ifdef SguanMotor1
    Sguan1.Foc.Period = 4249;

    Sguan1.Position.Kp = 0.15f;
    Sguan1.Position.Ki = 0.00000025f;
    Sguan1.Position.Kd = 0.0f;
    Sguan1.Position.OutMax = 0.35f;
    Sguan1.Position.OutMin = -0.35f;
    Sguan1.Position.D_Filter = 0.2f;
    Sguan1.Position.ErrLimltFlag = 0;
    Sguan1.Position.ErrMax = 0.0f;
    Sguan1.Position.ErrMin = 0.0f;

    Sguan1.Speed.Kp = 0.02226f;
    Sguan1.Speed.Ki = 0.000115f;
    Sguan1.Speed.Kd = 0.0f;
    Sguan1.Speed.OutMax = 0.9f;
    Sguan1.Speed.OutMin = -0.9f;
    Sguan1.Speed.D_Filter = 0.2f;
    Sguan1.Speed.ErrLimltFlag = 0;
    Sguan1.Speed.ErrMax = 0.0f;
    Sguan1.Speed.ErrMin = 0.0f;

    Sguan1.Id.Kp = 0.02226f;
    Sguan1.Id.Ki = 0.000115f;
    Sguan1.Id.Kd = 0.0f;
    Sguan1.Id.OutMax = 0.9f;
    Sguan1.Id.OutMin = -0.9f;
    Sguan1.Id.D_Filter = 0.2f;
    Sguan1.Id.ErrLimltFlag = 0;
    Sguan1.Id.ErrMax = 0.0f;
    Sguan1.Id.ErrMin = 0.0f;

    Sguan1.Id.Kp = 0.02226f;
    Sguan1.Id.Ki = 0.000115f;
    Sguan1.Id.Kd = 0.0f;
    Sguan1.Id.OutMax = 0.9f;
    Sguan1.Id.OutMin = -0.9f;
    Sguan1.Id.D_Filter = 0.2f;
    Sguan1.Id.ErrLimltFlag = 0;
    Sguan1.Id.ErrMax = 0.0f;
    Sguan1.Id.ErrMin = 0.0f;
    #endif // SguanMotor1
    #ifdef SguanMotor2
    Sguan2.Foc.Period = 4249;

    Sguan2.Position.Kp = 0.15f;
    Sguan2.Position.Ki = 0.00000025f;
    Sguan2.Position.Kd = 0.0f;
    Sguan2.Position.OutMax = 0.35f;
    Sguan2.Position.OutMin = -0.35f;
    Sguan2.Position.D_Filter = 0.2f;
    Sguan2.Position.ErrLimltFlag = 0;
    Sguan2.Position.ErrMax = 0.0f;
    Sguan2.Position.ErrMin = 0.0f;

    Sguan2.Speed.Kp = 0.02226f;
    Sguan2.Speed.Ki = 0.000115f;
    Sguan2.Speed.Kd = 0.0f;
    Sguan2.Speed.OutMax = 0.9f;
    Sguan2.Speed.OutMin = -0.9f;
    Sguan2.Speed.D_Filter = 0.2f;
    Sguan2.Speed.ErrLimltFlag = 0;
    Sguan2.Speed.ErrMax = 0.0f;
    Sguan2.Speed.ErrMin = 0.0f;

    Sguan2.Id.Kp = 0.02226f;
    Sguan2.Id.Ki = 0.000115f;
    Sguan2.Id.Kd = 0.0f;
    Sguan2.Id.OutMax = 0.9f;
    Sguan2.Id.OutMin = -0.9f;
    Sguan2.Id.D_Filter = 0.2f;
    Sguan2.Id.ErrLimltFlag = 0;
    Sguan2.Id.ErrMax = 0.0f;
    Sguan2.Id.ErrMin = 0.0f;

    Sguan2.Id.Kp = 0.02226f;
    Sguan2.Id.Ki = 0.000115f;
    Sguan2.Id.Kd = 0.0f;
    Sguan2.Id.OutMax = 0.9f;
    Sguan2.Id.OutMin = -0.9f;
    Sguan2.Id.D_Filter = 0.2f;
    Sguan2.Id.ErrLimltFlag = 0;
    Sguan2.Id.ErrMax = 0.0f;
    Sguan2.Id.ErrMin = 0.0f;
    #endif // SguanMotor2
    #ifdef SguanMotor3
    Sguan3.Foc.Period = 4249;

    Sguan3.Position.Kp = 0.15f;
    Sguan3.Position.Ki = 0.00000025f;
    Sguan3.Position.Kd = 0.0f;
    Sguan3.Position.OutMax = 0.35f;
    Sguan3.Position.OutMin = -0.35f;
    Sguan3.Position.D_Filter = 0.2f;
    Sguan3.Position.ErrLimltFlag = 0;
    Sguan3.Position.ErrMax = 0.0f;
    Sguan3.Position.ErrMin = 0.0f;

    Sguan3.Speed.Kp = 0.02226f;
    Sguan3.Speed.Ki = 0.000115f;
    Sguan3.Speed.Kd = 0.0f;
    Sguan3.Speed.OutMax = 0.9f;
    Sguan3.Speed.OutMin = -0.9f;
    Sguan3.Speed.D_Filter = 0.2f;
    Sguan3.Speed.ErrLimltFlag = 0;
    Sguan3.Speed.ErrMax = 0.0f;
    Sguan3.Speed.ErrMin = 0.0f;

    Sguan3.Id.Kp = 0.02226f;
    Sguan3.Id.Ki = 0.000115f;
    Sguan3.Id.Kd = 0.0f;
    Sguan3.Id.OutMax = 0.9f;
    Sguan3.Id.OutMin = -0.9f;
    Sguan3.Id.D_Filter = 0.2f;
    Sguan3.Id.ErrLimltFlag = 0;
    Sguan3.Id.ErrMax = 0.0f;
    Sguan3.Id.ErrMin = 0.0f;

    Sguan3.Id.Kp = 0.02226f;
    Sguan3.Id.Ki = 0.000115f;
    Sguan3.Id.Kd = 0.0f;
    Sguan3.Id.OutMax = 0.9f;
    Sguan3.Id.OutMin = -0.9f;
    Sguan3.Id.D_Filter = 0.2f;
    Sguan3.Id.ErrLimltFlag = 0;
    Sguan3.Id.ErrMax = 0.0f;
    Sguan3.Id.ErrMin = 0.0f;
    #endif // SguanMotor3
}


void SguanUser_Init(void) {
    /* Your code for initing TIM and gate driver and encoder and ADC here */
}

void SguanUser_Delay(uint32_t ms) {
    /* Your code for Delay_ms here */
}

uint32_t SguanUser_GetTick(void) {
    uint32_t Tick_num;
    /* Your code for getting time-tick here */
    return Tick_num;
}

uint32_t SguanUser_ReadADC_Raw(uint8_t Motor_CH,uint8_t Current_CH) {
    uint32_t ADC_num;
    switch (Motor_CH)
    {
    case 0:
        switch (Current_CH)
        {
        case 0:
            /* Your code for MotorA IA raw */
            break;
        case 1:
            /* Your code for MotorA IB raw */
            break;
        case 2:
            /* Your code for MotorA IC raw */
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (Current_CH)
        {
        case 0:
            /* Your code for MotorB IA raw */
            break;
        case 1:
            /* Your code for MotorB IB raw */
            break;
        case 2:
            /* Your code for MotorB IC raw */
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (Current_CH)
        {
        case 0:
            /* Your code for MotorC IA raw */
            break;
        case 1:
            /* Your code for MotorC IB raw */
            break;
        case 2:
            /* Your code for MotorC IC raw */
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (Current_CH)
        {
        case 0:
            /* Your code for MotorD IA raw */
            break;
        case 1:
            /* Your code for MotorD IB raw */
            break;
        case 2:
            /* Your code for MotorD IC raw */
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return ADC_num;
}

float SguanUser_Encoder_ReadRad(uint8_t Motor_CH) {
    float Rad_num;
    switch (Motor_CH)
    {
    case 0:
        /* Your code for getting encoder CH0 radian position */
        break;
    case 1:
        /* Your code for getting encoder CH1 radian position */
        break;
    case 2:
        /* Your code for getting encoder CH2 radian position */
        break;
    case 3:
        /* Your code for getting encoder CH3 radian position */
        break;
    default:
        break;
    }
    return Rad_num;
}

void SguanUser_PwmDuty_Set(uint8_t Motor_CH,uint8_t PWM_CH,uint16_t Duty) {
    switch (Motor_CH)
    {
    case 0:
        switch (PWM_CH)
        {
        case 0:
            /* Your code for MotorA PWM_CH0 duty set */
            break;
        case 1:
            /* Your code for MotorA PWM_CH1 duty set */
            break;
        case 2:
            /* Your code for MotorA PWM_CH2 duty set */
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (PWM_CH)
        {
        case 0:
            /* Your code for MotorB PWM_CH0 duty set */
            break;
        case 1:
            /* Your code for MotorB PWM_CH1 duty set */
            break;
        case 2:
            /* Your code for MotorB PWM_CH2 duty set */
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (PWM_CH)
        {
        case 0:
            /* Your code for MotorC PWM_CH0 duty set */
            break;
        case 1:
            /* Your code for MotorC PWM_CH1 duty set */
            break;
        case 2:
            /* Your code for MotorC PWM_CH2 duty set */
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (PWM_CH)
        {
        case 0:
            /* Your code for MotorD PWM_CH0 duty set */
            break;
        case 1:
            /* Your code for MotorD PWM_CH1 duty set */
            break;
        case 2:
            /* Your code for MotorD PWM_CH2 duty set */
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}




#endif // SGUANUSER_DATA_H
