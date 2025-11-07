#ifndef __FOC_H
#define __FOC_H

#include "main.h"
#include "data_parameters.h"

void Sguan_FocInit(void);
void Set_Duty(float d_u,float d_v,float d_w);
void SguanFOC_Position_OpenLoop(float target_angle,float Ud_Set);
void SguanFOC_Velocity_OpenLoop(float phi,float Uq_Set);
void Speed_PidInit(void);
void SguanFOC_Velocity_CloseLoop(float phi,float target_speed);

#endif // FOC_H
