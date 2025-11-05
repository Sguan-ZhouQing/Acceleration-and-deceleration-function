/*
 * @Author: 星必尘Sguan
 * @Date: 2025-11-04 23:01:12
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 09:50:42
 * @FilePath: \demo_SguanFOCv2.0\Software\motor_SVPWM.c
 * @Description: SVPWM应用函数书写
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "motor_SVPWM.h"

/**
 * @description: SVPWM函数实现
 * @param {float} phi
 * @param {float} d
 * @param {float} q
 * @param {float} *d_u
 * @param {float} *d_v
 * @param {float} *d_w
 * @return {*}
 */
void SVPWM(float phi, float d, float q, float *d_u, float *d_v, float *d_w) {
    d = Sg_Limit(d,1,-1);   // 限幅函数
    q = Sg_Limit(q,1,-1);
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
    const int K_to_sector[] = {4, 6, 5, 5, 3, 1, 2, 2};
    float sin_phi,cos_phi;
    fast_sin_cos(phi,&sin_phi,&cos_phi);
    float alpha,beta;
    ipark(&alpha, &beta, d, q, sin_phi, cos_phi);

    bool A = beta > 0;
    bool B = fabsf(beta) > SQRT3 * fabsf(alpha);
    bool C = alpha > 0;
    int K = 4 * A + 2 * B + C;
    int sector = K_to_sector[K];

    float angle_data0 = sector * rad60;
    float angle_data1 = angle_data0 - rad60;
    float sin_m,cos_m,sin_n,cos_n;
    fast_sin_cos(angle_data0,&sin_m,&cos_m);
    fast_sin_cos(angle_data1,&sin_n,&cos_n);

    float t_m = sin_m * alpha - cos_m * beta;
    float t_n = beta * cos_n - alpha * sin_n;
    float t_0 = 1 - t_m - t_n;
    *d_u = t_m * v[sector - 1][0] + t_n * v[sector % 6][0] + t_0 / 2;
    *d_v = t_m * v[sector - 1][1] + t_n * v[sector % 6][1] + t_0 / 2;
    *d_w = t_m * v[sector - 1][2] + t_n * v[sector % 6][2] + t_0 / 2;
}


/**
 * @description: 克拉克变换
 * @param {float} *i_alpha
 * @param {float} *i_beta
 * @param {float} i_a
 * @param {float} i_b
 * @return {*}
 */
void clarke(float *i_alpha,float *i_beta,float i_a,float i_b) {
    *i_alpha = i_a;
    *i_beta = (i_a + 2 * i_b) * 0.5773502691896257f;
}

/**
 * @description: 帕克变换
 * @param {float} *i_d
 * @param {float} *i_q
 * @param {float} i_alpha
 * @param {float} i_beta
 * @param {float} sine
 * @param {float} cosine
 * @return {*}
 */
void park(float *i_d,float *i_q,float i_alpha,float i_beta,float sine,float cosine) {
    *i_d = i_alpha * cosine + i_beta * sine;
    *i_q = i_beta * cosine - i_alpha * sine;
}

/**
 * @description: 帕克“逆”变换
 * @param {float} *u_alpha
 * @param {float} *u_beta
 * @param {float} u_d
 * @param {float} u_q
 * @param {float} sine
 * @param {float} cosine
 * @return {*}
 */
void ipark(float *u_alpha,float *u_beta,float u_d,float u_q,float sine,float cosine) {
    *u_alpha = u_d * cosine - u_q * sine;
    *u_beta = u_q * cosine + u_d * sine;
}
