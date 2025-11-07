/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-26 15:32:26
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-07 12:11:09
 * @FilePath: \demo_SguanFOCv2.0\Hardware\printf.c
 * @Description: 使用USART串口收发和数据处理
 * @Key_GPIO: Many;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "printf.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
// 引入外部函数
#include "foc.h"
#include "motor_pid.h"
extern PID_STRUCT SguanVal;


// 定义串口缓存区
extern UART_HandleTypeDef huart1;
uint8_t G431_readBuffer[G431_BUFFER_SIZE];
// 串口接收缓冲数组
uint8_t RxBuffer[1];        // 串口接收缓冲
uint16_t RxLine = 0;        // 指令长度
uint8_t DataBuff[200];      // 指令内容
// 全局可调变量
float Adjustable_Data = 0.0f;


// 支持printf函数，而不需要选择MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE { 
	int handle; 
}; 
// 支持printf函数，而不需要选择MicroLIB
FILE __stdout;
//定义_sys_exit避免使用半主机模式
void _sys_exit(int x) { 
	x = x; 
} 
#endif
//串口重定向函数printf，不使用到MicroLIB
int fputc(int ch,FILE *f) {
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}


//串口主程序初始化
void UART_Init(void)
{
    // 重新启动接收，使用Ex函数，接收不定长数据
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, G431_readBuffer, sizeof(G431_readBuffer));
    // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

// 数据解析函数
static float Get_Data(void) {
    uint8_t data_Start_Num = 0;
    uint8_t data_End_Num = 0;
    uint8_t minus_Flag = 0;
    float data_return = 0;
    
    // 查找等号和问号的位置
    for(uint8_t i = 0; i < 200; i++) {
        if(DataBuff[i] == '=') data_Start_Num = i + 1;
        if(DataBuff[i] == '?') {
            data_End_Num = i - 1;
            break;
        }
    }
    
    if(DataBuff[data_Start_Num] == '-') {
        data_Start_Num += 1;
        minus_Flag = 1;
    }
    
    data_return = 0;
    uint8_t decimal_point = 0;
    uint8_t decimal_digits = 0;  // 记录小数位数
    
    // 先找到小数点的位置
    for(uint8_t i = data_Start_Num; i <= data_End_Num; i++) {
        if(DataBuff[i] == '.') {
            decimal_point = i;
            break;
        }
    }
    
    // 处理整数部分和小数部分
    for(uint8_t i = data_Start_Num; i <= data_End_Num; i++) {
        if(DataBuff[i] == '.') {
            continue;  // 跳过小数点
        }
        
        if(i < decimal_point || decimal_point == 0) {
            // 整数部分
            data_return = data_return * 10 + (DataBuff[i] - '0');
        } else {
            // 小数部分：根据小数位的位置计算权重
            decimal_digits++;
            float decimal_weight = 1.0f;
            for(uint8_t j = 0; j < decimal_digits; j++) {
                decimal_weight *= 0.1f;
            }
            data_return += (DataBuff[i] - '0') * decimal_weight;
        }
    }
    
    if(minus_Flag == 1) data_return = -data_return;
    return data_return;
}


// PID调整函数（需要根据你的实际PID结构体定义进行调整）
static void PID_Adjust(uint8_t Motor_n) {
    float data_Get = Get_Data();
    // 这里需要根据你的实际PID结构体进行修改
    // 示例代码：
    if(Motor_n == 1) {
        if(DataBuff[0]=='A' && DataBuff[1]=='1')
        {
            // PID_Pos.P = data_Get;
            Adjustable_Data = data_Get;
        }
        if(DataBuff[0]=='P' && DataBuff[1]=='2')
        {
            // PID_Pos.P = data_Get;
//            SguanFOC.Velocity_PID.Kp = data_Get;
            SguanVal.Kp = data_Get;
        }
        if(DataBuff[0]=='I' && DataBuff[1]=='2')
        {
            // PID_Pos.P = data_Get;
//            SguanFOC.Velocity_PID.Ki = data_Get;
            SguanVal.Ki = data_Get;
        }
        if(DataBuff[0]=='D' && DataBuff[1]=='2')
        {
            // PID_Pos.P = data_Get;
//            SguanFOC.Velocity_PID.Kd = data_Get;
            SguanVal.Kd = data_Get;
        }
        // 其他PID参数调整逻辑...
    }
}


/**
 * @description: 处理接收到的数据（回调函数）
 * @param {UART_HandleTypeDef} *huart
 * @param {uint16_t} Size
 * @return {*}
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        // 处理接收到的数据
        for(uint16_t i = 0; i < Size; i++) {
            RxLine++;
            if(RxLine <= sizeof(DataBuff)) {
                DataBuff[RxLine-1] = G431_readBuffer[i];
            }
            if(G431_readBuffer[i] == 0x3F) { // 结束标志
                PID_Adjust(1);
                memset(DataBuff, 0, sizeof(DataBuff));
                RxLine = 0;
                break;
            }
        }
        // 重新启动接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, G431_readBuffer, sizeof(G431_readBuffer));
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }
}

