#ifndef __CURRENT_H
#define __CURRENT_H

#include "stm32g4xx_hal.h"

// ADC采样数据结构体
typedef struct {
    float U_Current;        // U相电流 (A)
    float W_Current;        // W相电流 (A)
    float Bus_Voltage;      // 母线电压 (V)
    float Potentiometer;    // 滑动变阻器电压 (V)
    float U_Phase_Voltage;  // U相端电压 (V)
    float V_Phase_Voltage;  // V相端电压 (V)
    float W_Phase_Voltage;  // W相端电压 (V)
    float Temperature;      // 温度传感器电压 (V)
} ADC_SampleData_t;

// ADC状态机
typedef enum {
    ADC_STATE_IDLE = 0,
    ADC_STATE_INJECTED_STARTED,
    ADC_STATE_REGULAR_STARTED,
    ADC_STATE_READY
} ADC_State_t;

// 外部变量声明
extern ADC_SampleData_t ADC_Data;
extern ADC_State_t ADC_State;
extern volatile uint8_t ADC_ConversionComplete;

// 函数声明
void ADC_Init(void);
void ADC_Start_Conversion_Sequence(void);
void ADC_Process_Conversion_Sequence(void);
void ADC_Get_Injected_Values(void);
void ADC_Get_Regular_Values(void);
void ADC_Process_All_Data(void);
uint8_t ADC_Is_Data_Ready(void);


#endif // CURRENT_H
