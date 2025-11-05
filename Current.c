/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:24:49
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-06 00:59:15
 * @FilePath: \demo_SguanFOCv2.0\Hardware\Current.c
 * @Description: 电流采样实现
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Current.h"
// 外部变量包含
extern ADC_HandleTypeDef hadc2;


// ADC采样数据全局变量
ADC_SampleData_t ADC_Data = {0};
// ADC原始采样值数组
volatile uint16_t ADC_InjectedValues[4] = {0};
volatile uint16_t ADC_RegularValues[4] = {0};
// ADC状态控制
ADC_State_t ADC_State = ADC_STATE_IDLE;
volatile uint8_t ADC_ConversionComplete = 0;

// 电压和电流换算系数
#define VOLTAGE_REF                3.3f
#define ADC_RESOLUTION             4095.0f
#define BUS_VOLTAGE_DIVIDER_RATIO  (1.0f/0.4489f)
#define PHASE_VOLTAGE_DIVIDER_RATIO (1.0f/0.047619f)
#define SHUNT_RESISTANCE           0.005f
#define OPAMP_GAIN                 10.0f


// ADC初始化
void ADC_Init(void) {
    // 校准ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    // 启用ADC全局中断
    __HAL_ADC_ENABLE_IT(&hadc2, ADC_IT_JEOC | ADC_IT_EOC);
}

// 启动ADC转换序列（在定时器中断中调用）
void ADC_Start_Conversion_Sequence(void) {
    if (ADC_State == ADC_STATE_IDLE) {
        ADC_State = ADC_STATE_INJECTED_STARTED;
        ADC_ConversionComplete = 0;
        // 启动注入组转换（非阻塞）
        HAL_ADCEx_InjectedStart_IT(&hadc2);
    }
}

// 处理ADC转换序列（在主循环中调用）
void ADC_Process_Conversion_Sequence(void) {
    if (ADC_ConversionComplete) {
        // 处理所有数据
        ADC_Process_All_Data();        
        // 重置状态
        ADC_State = ADC_STATE_IDLE;
        ADC_ConversionComplete = 0;

        // 这里可以触发FOC计算等其他任务
    }
}

// 获取ADC注入组采样值
void ADC_Get_Injected_Values(void) {
    ADC_InjectedValues[0] = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1); // IN11
    ADC_InjectedValues[1] = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_2); // IN3
    ADC_InjectedValues[2] = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_3); // IN17
    ADC_InjectedValues[3] = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_4); // IN12
}

// 获取ADC规则组采样值
void ADC_Get_Regular_Values(void) {
    ADC_RegularValues[0] = HAL_ADC_GetValue(&hadc2); // IN5
    ADC_RegularValues[1] = HAL_ADC_GetValue(&hadc2); // IN4  
    ADC_RegularValues[2] = HAL_ADC_GetValue(&hadc2); // IN13
    ADC_RegularValues[3] = HAL_ADC_GetValue(&hadc2); // IN15
}

// 处理所有ADC数据并进行换算
void ADC_Process_All_Data(void) {
    float adc_voltage;
    // 处理注入组数据
    adc_voltage = (ADC_InjectedValues[0] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.Bus_Voltage = adc_voltage * BUS_VOLTAGE_DIVIDER_RATIO;
    
    adc_voltage = (ADC_InjectedValues[1] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.U_Current = (adc_voltage / OPAMP_GAIN) / SHUNT_RESISTANCE;
    
    adc_voltage = (ADC_InjectedValues[2] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.W_Current = (adc_voltage / OPAMP_GAIN) / SHUNT_RESISTANCE;
    
    ADC_Data.Potentiometer = (ADC_InjectedValues[3] / ADC_RESOLUTION) * VOLTAGE_REF;
    
    // 处理规则组数据
    adc_voltage = (ADC_RegularValues[0] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.W_Phase_Voltage = adc_voltage * PHASE_VOLTAGE_DIVIDER_RATIO;
    
    adc_voltage = (ADC_RegularValues[1] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.V_Phase_Voltage = adc_voltage * PHASE_VOLTAGE_DIVIDER_RATIO;
    
    adc_voltage = (ADC_RegularValues[2] / ADC_RESOLUTION) * VOLTAGE_REF;
    ADC_Data.U_Phase_Voltage = adc_voltage * PHASE_VOLTAGE_DIVIDER_RATIO;
    
    ADC_Data.Temperature = (ADC_RegularValues[3] / ADC_RESOLUTION) * VOLTAGE_REF;
}

// 检查ADC数据是否就绪
uint8_t ADC_Is_Data_Ready(void) {
    return ADC_ConversionComplete;
}

// ADC中断回调函数（注入）
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC2) {
        if (ADC_State == ADC_STATE_INJECTED_STARTED) {
            // 获取注入组数据
            ADC_Get_Injected_Values();
            // 切换到规则组转换
            ADC_State = ADC_STATE_REGULAR_STARTED;
            HAL_ADC_Start_IT(&hadc2);
        }
    }
}

// ADC中断回调函数（规则）
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC2) {
        if (ADC_State == ADC_STATE_REGULAR_STARTED) {
            // 获取规则组数据
            ADC_Get_Regular_Values();
            // 标记转换完成
            ADC_State = ADC_STATE_READY;
            ADC_ConversionComplete = 1;
            // 停止ADC转换
            HAL_ADC_Stop(&hadc2);
            HAL_ADCEx_InjectedStop(&hadc2);
        }
    }
}

