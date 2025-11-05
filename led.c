/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:29:19
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 21:49:52
 * @FilePath: \demo_SguanFOCv2.0\Hardware\led.c
 * @Description: RGB灯闪烁函数书写
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "led.h"

static uint8_t LED_TimeNum = 0;
static uint8_t Freq_div = 0;
static uint8_t Time_Out_Count = 200;

void LED_Tick(void) {
    switch (LED_TimeNum) {
    case 0:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        break;
    case 1:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        break;
    case 2:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);
        break;
    case 3:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);
        break;
    case 4:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        break;
    case 5:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);
        break;
    case 6:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        break;
    case 7:
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);
        break;
    case 8:
        // HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_14);
        break;
    default:
        break;
    }
    Freq_div++;
    if (Freq_div == Time_Out_Count) {
        LED_TimeNum++;
        Freq_div = 0;
    }
    if (LED_TimeNum == 9) {
        LED_TimeNum = 0;
    }
}
