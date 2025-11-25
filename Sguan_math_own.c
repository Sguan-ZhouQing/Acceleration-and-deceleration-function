// Sguan_math.h 中添加
typedef int32_t q15_t;     // Q15格式：1位符号，15位小数
typedef int32_t q31_t;     // Q31格式：1位符号，31位小数

#define Q15_SHIFT 15
#define Q31_SHIFT 31

// Q15常量定义
#define Q15_PI          (102944)        // 3.1415926535 * 2^15
#define Q15_2PI         (205887)        // 2*PI
#define Q15_INV_SQRT3   (18918)         // 1/sqrt(3) * 2^15
#define Q15_SQRT3_2     (28377)         // sqrt(3)/2 * 2^15
#define Q15_ONE         (32768)         // 1.0 * 2^15

// 基本定点运算
#define Q15_MUL(a, b)   ((q15_t)(((int32_t)(a) * (int32_t)(b)) >> Q15_SHIFT))
#define Q15_DIV(a, b)   ((q15_t)(((int32_t)(a) << Q15_SHIFT) / (b)))
#define Q15_ADD(a, b)   ((a) + (b))
#define Q15_SUB(a, b)   ((a) - (b))
#define Q15_FROM_FLOAT(x) ((q15_t)((x) * (1 << Q15_SHIFT)))
#define Q15_TO_FLOAT(x) ((float)(x) / (1 << Q15_SHIFT))





// 定点化快速sin/cos函数
static q15_t q15_f1(q15_t x) {
    // 使用Q15格式的多项式系数
    q15_t u = 4;        // 1.3528548e-10f * 2^15
    u = Q15_MUL(u, x) + (-81);      // -2.4703144e-08f
    u = Q15_MUL(u, x) + (90);       // 2.7532926e-06f
    u = Q15_MUL(u, x) + (-650);     // -0.00019840381f
    u = Q15_MUL(u, x) + (2730);     // 0.0083333179f
    return Q15_MUL(u, x) + (-5461); // -0.16666666f
}
static q15_t q15_f2(q15_t x) {
    q15_t u = 6;        // 1.7290616e-09f
    u = Q15_MUL(u, x) + (-89);      // -2.7093486e-07f
    u = Q15_MUL(u, x) + (812);      // 2.4771643e-05f
    u = Q15_MUL(u, x) + (-4551);    // -0.0013887906f
    u = Q15_MUL(u, x) + (13653);    // 0.041666519f
    return Q15_MUL(u, x) + (-16384);// -0.49999991f
}
q15_t q15_fast_sin(q15_t x) {
    // 将角度归一化到[-π, π]
    while (x > Q15_PI) x -= Q15_2PI;
    while (x < -Q15_PI) x += Q15_2PI;
    
    q15_t x2 = Q15_MUL(x, x);
    q15_t x3 = Q15_MUL(x, x2);
    
    return x + Q15_MUL(x3, q15_f1(x2));
}
q15_t q15_fast_cos(q15_t x) {
    // 将角度归一化到[-π, π]
    while (x > Q15_PI) x -= Q15_2PI;
    while (x < -Q15_PI) x += Q15_2PI;
    
    q15_t x2 = Q15_MUL(x, x);
    
    return Q15_ONE + Q15_MUL(x2, q15_f2(x2));
}


// 定点化PID结构体
typedef struct {
    q15_t Kp, Ki, Kd;
    q15_t Ref, Fbk, Out;
    q15_t Err, ErrLast;
    q15_t Ui;
    q15_t OutMax, OutMin;
    q15_t LastDerivative;
    q15_t D_Filter;
} Q15_PID_STRUCT;
void q15_pid_control(Q15_PID_STRUCT *p) {
    p->Err = p->Ref - p->Fbk;
    // 积分抗饱和
    if(p->Out != p->OutMax && p->Out != p->OutMin) {
        p->Ui += p->Err;
        // 积分限幅防止溢出
        if(p->Ui > (p->OutMax << 1)) p->Ui = p->OutMax << 1;
        if(p->Ui < (p->OutMin << 1)) p->Ui = p->OutMin << 1;
    }
    // 微分项（带滤波）
    q15_t derivative_term = p->Err - p->ErrLast;
    if(p->D_Filter > 0) {
        derivative_term = Q15_MUL(p->D_Filter, derivative_term) + 
                         Q15_MUL(Q15_ONE - p->D_Filter, p->LastDerivative);
        p->LastDerivative = derivative_term;
    }
    // PID计算
    p->Out = Q15_MUL(p->Kp, p->Err) + 
             Q15_MUL(p->Ki, p->Ui) + 
             Q15_MUL(p->Kd, derivative_term);
    
    p->ErrLast = p->Err;
    // 输出限幅
    if(p->Out > p->OutMax) p->Out = p->OutMax;
    if(p->Out < p->OutMin) p->Out = p->OutMin;
}


// 定点化Clarke变换
void q15_clarke(q15_t *i_alpha, q15_t *i_beta, q15_t i_a, q15_t i_b) {
    *i_alpha = i_a;
    *i_beta = Q15_MUL(i_a + (i_b << 1), Q15_INV_SQRT3);
}
// 定点化Park变换
void q15_park(q15_t *i_d, q15_t *i_q, q15_t i_alpha, q15_t i_beta, 
              q15_t sine, q15_t cosine) {
    *i_d = Q15_MUL(i_alpha, cosine) + Q15_MUL(i_beta, sine);
    *i_q = Q15_MUL(i_beta, cosine) - Q15_MUL(i_alpha, sine);
}
// 定点化逆Park变换
void q15_ipark(q15_t *u_alpha, q15_t *u_beta, q15_t u_d, q15_t u_q,
               q15_t sine, q15_t cosine) {
    *u_alpha = Q15_MUL(u_d, cosine) - Q15_MUL(u_q, sine);
    *u_beta = Q15_MUL(u_q, cosine) + Q15_MUL(u_d, sine);
}


// 定点化SVPWM
void q15_svpwm(q15_t phi, q15_t d, q15_t q, 
               q15_t *d_u, q15_t *d_v, q15_t *d_w) {
    q15_t sin_phi, cos_phi;
    q15_t u_alpha, u_beta;
    // 获取sin/cos值
    sin_phi = q15_fast_sin(phi);
    cos_phi = q15_fast_cos(phi);
    // 逆Park变换
    q15_ipark(&u_alpha, &u_beta, d, q, sin_phi, cos_phi);
    // 计算三相参考电压（简化版本）
    q15_t Uref1 = u_beta;
    q15_t Uref2 = Q15_MUL(u_alpha, Q15_SQRT3_2) - (u_beta >> 1);
    q15_t Uref3 = -Q15_MUL(u_alpha, Q15_SQRT3_2) - (u_beta >> 1);
    // 扇区判断和占空比计算（简化实现）
    // 这里需要根据实际SVPWM算法完善
    // 归一化到[0, Q15_ONE]范围
    *d_u = (u_alpha + Q15_ONE) >> 1;
    *d_v = (u_beta + Q15_ONE) >> 1;
    *d_w = Q15_ONE - *d_u - *d_v;
    // 限幅
    if(*d_u > Q15_ONE) *d_u = Q15_ONE;
    if(*d_u < 0) *d_u = 0;
    if(*d_v > Q15_ONE) *d_v = Q15_ONE;
    if(*d_v < 0) *d_v = 0;
    if(*d_w > Q15_ONE) *d_w = Q15_ONE;
    if(*d_w < 0) *d_w = 0;
}


// 在FOC主循环中使用定点数
void SguanFOC_GeneratePWM_Loop_FixedPoint(void) {
    #ifdef SguanMotor0
    // 将浮点参数转换为定点数
    q15_t el_rad = Q15_FROM_FLOAT(Sguan0.Encoder.Real_El_Rad);
    q15_t id_out = Q15_FROM_FLOAT(Sguan0.Foc.Id_Out);
    q15_t iq_out = Q15_FROM_FLOAT(Sguan0.Foc.Iq_Out);
    q15_t du, dv, dw;
    // 定点化SVPWM
    q15_svpwm(el_rad, id_out, iq_out, &du, &dv, &dw);
    // 转换回浮点数给后续处理（如果需要）
    Sguan0.Foc.Input_du = Q15_TO_FLOAT(du);
    Sguan0.Foc.Input_dv = Q15_TO_FLOAT(dv);
    Sguan0.Foc.Input_dw = Q15_TO_FLOAT(dw);
    Sguan_SetDuty(0, Sguan0.Foc.Input_du, Sguan0.Foc.Input_dv, Sguan0.Foc.Input_dw);
    #endif
}