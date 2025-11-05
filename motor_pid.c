/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 23:00:30
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 00:53:50
 * @FilePath: \demo_SguanFOCv2.0\Software\motor_pid.c
 * @Description: PID运算函数实现
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "motor_pid.h"

/**
 * @description: PID计算抗积分饱和；
 * @param {PID_STRUCT} *p
 * @return {*}
 */
void PID_Control(PID_STRUCT *p){
    p->Err = p->Ref - p->Fbk;
    // 误差限幅
    if(p->ErrLimltFlag == 1) {
        p->Err = Sg_Limit(p->Err, p->ErrMax, p->ErrMin);
    }
    // 积分抗饱和
    if(p->Out != p->OutMax && p->Out != p->OutMin) {
        p->Ui += p->Err;
    }
    // PID计算
    p->Out = p->Kp * p->Err + p->Ki * p->Ui + p->Kd * (p->Err - p->ErrLast);
    p->ErrLast = p->Err;
    // 输出限幅
    p->Out = Sg_Limit(p->Out, p->OutMax, p->OutMin);
}

