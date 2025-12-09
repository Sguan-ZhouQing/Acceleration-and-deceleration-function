/*
 * @Author: 星必尘Sguan
 * @Date: 2025-12-09 19:53:31
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-12-09 21:22:04
 * @FilePath: \demo_ButterFly\Hardware\nRF_printf.c
 * @Description: printf函数
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "nRF_printf.h"
#include "nRF24L01.h"

static int floats_to_ascii_with_length(float *nums, uint8_t num_count, uint8_t *data, size_t data_size, int decimal_places) {
    char buffer[256] = {0};
    char format[8];
    char temp[32];
    
    if (num_count <= 0 || nums == NULL || data == NULL) {
        return -1;
    }
    
    // 构建格式字符串
    if (decimal_places > 0) {
        snprintf(format, sizeof(format), "%%.%df", decimal_places);
    } else {
        snprintf(format, sizeof(format), "%%.0f");
    }
    
    // 构建完整的字符串，用逗号分隔
    for (int i = 0; i < num_count; i++) {
        if (i > 0) {
            strcat(buffer, ",");
        }
        snprintf(temp, sizeof(temp), format, nums[i]);
        if (strlen(buffer) + strlen(temp) + 1 >= sizeof(buffer)) {
            return -1;
        }
        strcat(buffer, temp);
    }
    
    // 添加标准换行符
    strcat(buffer, "\n");  // 添加标准的换行符
    
    // 计算总长度
    int str_len = (int)strlen(buffer);
    
    // 检查目标数组是否足够大
    if (data_size < (size_t)(str_len + 1)) {  // +1为长度字节
        return -1;
    }
    
    // 第一个字节存储字符串的长度
    data[0] = (uint8_t)str_len;
    
    // 复制字符串（包含换行符）
    memcpy(&data[1], buffer, str_len);
    
    return str_len + 1;  // 返回总字节数
}

void nRF_Printf(float *num, uint8_t count) {
    uint8_t data[32] = {0};
    int len = floats_to_ascii_with_length(
        num, 
        count,          // 数字个数
        data, 
        sizeof(data), 
        2               // 保留2位小数
    );
    
    if (len > 0) {
        NRF24L01_SendBuf(data);
    }
}
