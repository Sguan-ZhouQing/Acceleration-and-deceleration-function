/*
 * @Author: 星必尘Sguan
 * @Date: 2025-12-09 19:53:31
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-12-10 01:24:12
 * @FilePath: \demo_ButterFly\Hardware\nRF_printf.c
 * @Description: printf函数
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "nRF_printf.h"
#include "nRF24L01.h"

float nRF_DataA,nRF_DataB,nRF_DataC;

// 浮点数to十六进制码
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

/**
 * @brief 将特定格式的 uint8_t 数组转换回浮点数数组
 * @param data 输入的 uint8_t 数组（第一个字节是长度，最后一个字节是'\n'）
 * @param max_floats 浮点数数组的最大容量
 * @param floats 输出浮点数数组
 * @return int 成功解析的浮点数个数，-1表示错误
 */
static int ascii_to_floats(const uint8_t *data, uint8_t max_floats, float *floats) {
    if (data == NULL || floats == NULL || max_floats == 0) {
        return -1;
    }
    // 第一个字节是数据长度（不包括长度字节本身）
    uint8_t str_len = data[0];
    // 验证数据长度
    if (str_len == 0) {
        return -1;
    }
    // 验证最后一个字节是否为换行符
    if (data[str_len] != '\n') {  // str_len 索引指向最后一个字节（因为长度字节是 data[0]）
        return -1;
    }
    // 创建临时字符串缓冲区（不包括长度字节和换行符）
    char str_buffer[256] = {0};
    // 复制ASCII字符串（从 data[1] 到 data[str_len-1]）
    memcpy(str_buffer, &data[1], str_len);
    str_buffer[str_len] = '\0';  // 确保字符串以NULL结尾
    // 使用逗号分隔字符串
    char *token = strtok(str_buffer, ",");
    int count = 0;
    while (token != NULL && count < max_floats) {
        // 将字符串转换为浮点数
        floats[count] = (float)atof(token);
        count++;
        
        // 获取下一个token
        token = strtok(NULL, ",");
    }
    return count;
}

// 发送浮点数据（发送）
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

// 定时器中断每1ms中调用（接收）
void nRF_Tick(void) {
    float floats[3];
    uint8_t Buff_RX[32];
    if (NRF24L01_Get_Value_Flag() == 0) {
        NRF24L01_GetRxBuf(Buff_RX);
        ascii_to_floats(Buff_RX,3,floats);
        nRF_DataA = floats[0];
        nRF_DataB = floats[1];
        nRF_DataC = floats[2];
    }
}

