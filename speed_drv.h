#ifndef __SPEED_DRV_H
#define __SPEED_DRV_H

#include "stm32g4xx_hal.h"
#include "data_parameters.h"
#include "fast_sin.h"

typedef struct
{  
	float   StartSpeed;   //初始速度    
	float   EndSpeed;     //末速度
	float   AccSpeed;     //加速度
	float   SumSpeed;     //速度增量
	float   DecSpeed;     //减速度
	float   SpeedOut;     //输出目标速度
	uint8_t FinishFlag;   //加减速完成标志
}TSHAPEDACCDEC_STRUCT;

typedef struct
{  
    float   StartSpeed;   // 初始速度
    float   EndSpeed;     // 目标速度
    float   MaxAcc;       // 最大加速度
    float   Jerk;         // 加加速度（加速度的变化率）
    float   CurrentSpeed; // 当前速度
    float   CurrentAcc;   // 当前加速度
    float   TotalTime;    // 总时间估算
    float   ElapsedTime;  // 已运行时间
    uint8_t Phase;        // 阶段标志 (0:加速段, 1:减速段, 2:完成)
    uint8_t FinishFlag;   // 完成标志
} SCURVE_STRUCT;

#endif // SPEED_DRV_H
