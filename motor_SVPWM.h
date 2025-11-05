#ifndef __MOTOR_SVPWM_H
#define __MOTOR_SVPWM_H

#include "fast_sin.h"
#include "math.h"
#include "data_parameters.h"

void SVPWM(float phi, float d, float q, float *d_u, float *d_v, float *d_w);
void clarke(float *i_alpha,float *i_beta,float i_a,float i_b);
void park(float *i_d,float *i_q,float i_alpha,float i_beta,float sine,float cosine);
void ipark(float *u_alpha,float *u_beta,float u_d,float u_q,float sine,float cosine);

#endif // MOTOR_SVPWM_H
