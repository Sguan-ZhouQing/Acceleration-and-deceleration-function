/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 19:31:55
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 01:37:13
 * @FilePath: \demo_SguanFOCv2.0\Software\speed_drv.c
 * @Description: T型和S型“非突变”加速函数设计
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "speed_drv.h"

/**
 * @description: T型加速函数实现
 * @param {TSHAPEDACCDEC_STRUCT} *p
 * @return {*}
 */
void T_Shaped_Acc_Dec(TSHAPEDACCDEC_STRUCT *p) {
	if(p->FinishFlag == 0) {
		if((p->EndSpeed - p->StartSpeed) > 0) {
			p->SumSpeed = p->SumSpeed + p->AccSpeed;
			p->SpeedOut = p->StartSpeed + p->SumSpeed;
			if(p->SpeedOut >= p->EndSpeed) {
				p->SpeedOut = p->EndSpeed;
				p->FinishFlag = 1;
			}
		}
		if((p->EndSpeed - p->StartSpeed) < 0) {
			p->SumSpeed = p->SumSpeed - p->AccSpeed;
			p->SpeedOut = p->StartSpeed + p->SumSpeed;
			if(p->SpeedOut <= p->EndSpeed) {
				p->SpeedOut = p->EndSpeed;
				p->FinishFlag = 1;
			}
		}
		if(p->FinishFlag == 1) {
	  	p->SumSpeed = 0;
		}			
	}	
}

/**
 * @description: S型曲线加减速（修复版）
 * @param {SCURVE_STRUCT} *p
 * @return {*}
 */
void S_Curve_Acc_Dec(SCURVE_STRUCT *p) {
    if(p->FinishFlag == 1) {
        return;
    }
    
    float speed_diff = p->EndSpeed - p->StartSpeed;
    float total_speed_change = fabsf(speed_diff);
    
    // 重新计算总时间（更准确的方法）
    if(p->TotalTime == 0) {
        // 基于最大加速度和加加速度计算总时间
        float T1 = p->MaxAcc / p->Jerk;  // 加速段时间
        float S1 = 0.5f * p->Jerk * T1 * T1;  // 加速段速度变化
        if(total_speed_change <= 2 * S1) {
            // 无法达到最大加速度的情况
            T1 = sqrtf(total_speed_change / p->Jerk);
            p->TotalTime = 2 * T1;
        } else {
            // 能达到最大加速度的情况
            float S2 = total_speed_change - 2 * S1;
            float T2 = S2 / p->MaxAcc;
            p->TotalTime = 2 * T1 + T2;
        }
    }
    
    p->ElapsedTime += 0.001f;  // 每次调用增加1ms
    float t = p->ElapsedTime;
    
    if(t >= p->TotalTime) {
        p->CurrentSpeed = p->EndSpeed;
        p->CurrentAcc = 0;
        p->FinishFlag = 1;
        return;
    }
    
    float T1 = p->MaxAcc / p->Jerk;  // 加加速段时间
    float S1 = 0.5f * p->Jerk * T1 * T1;  // 加加速段速度变化
    
    if(total_speed_change <= 2 * S1) {
        // 情况1：无法达到最大加速度
        T1 = sqrtf(total_speed_change / p->Jerk);
        float T_total = 2 * T1;
        
        if(t < T1) {
            // 第一阶段：加速度增加
            p->CurrentAcc = p->Jerk * t;
            p->CurrentSpeed = p->StartSpeed + 0.5f * p->Jerk * t * t;
        } else {
            // 第二阶段：加速度减小
            float t2 = t - T1;
            p->CurrentAcc = p->Jerk * (2 * T1 - t) / T1;
            p->CurrentSpeed = p->StartSpeed + p->Jerk * T1 * t2 - 0.5f * p->Jerk * t2 * t2 + 0.5f * p->Jerk * T1 * T1;
        }
    } else {
        // 情况2：能达到最大加速度（完整S曲线）
        float S2 = total_speed_change - 2 * S1;
        float T2 = S2 / p->MaxAcc;
        float T_total = 2 * T1 + T2;
        
        if(t < T1) {
            // 第一阶段：加加速段
            p->CurrentAcc = p->Jerk * t;
            p->CurrentSpeed = p->StartSpeed + 0.5f * p->Jerk * t * t;
        } else if(t < T1 + T2) {
            // 第二阶段：匀加速段
            p->CurrentAcc = p->MaxAcc;
            float t2 = t - T1;
            p->CurrentSpeed = p->StartSpeed + S1 + p->MaxAcc * t2;
        } else {
            // 第三阶段：减加速段
            float t3 = t - T1 - T2;
            p->CurrentAcc = p->MaxAcc - p->Jerk * t3;
            p->CurrentSpeed = p->StartSpeed + S1 + S2 + p->MaxAcc * t3 - 0.5f * p->Jerk * t3 * t3;
        }
    }
    
    // 确保速度方向正确
    if(speed_diff < 0) {
        p->CurrentSpeed = p->StartSpeed - (p->CurrentSpeed - p->StartSpeed);
        p->CurrentAcc = -p->CurrentAcc;
    }
    
    // 边界检查
    if((speed_diff > 0 && p->CurrentSpeed >= p->EndSpeed) || 
       (speed_diff < 0 && p->CurrentSpeed <= p->EndSpeed)) {
        p->CurrentSpeed = p->EndSpeed;
        p->CurrentAcc = 0;
        p->FinishFlag = 1;
    }
}
