/*
 * @Author: 星必尘Sguan
 * @Date: 2025-04-21 17:02:15
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-11-05 00:09:25
 * @FilePath: \demo_SguanFOCv2.0\Underware\fast_sin.c
 * @Description: 不丢失太多精度的"sin的快速运算"函数;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "fast_sin.h"

/**
 * @description: f1 和 f2（多项式逼近）
 * @param {float} x
 * @return {*}
 */
float f1(float x) {
  float u = 1.3528548e-10f;
  u = u * x + -2.4703144e-08f;
  u = u * x + 2.7532926e-06f;
  u = u * x + -0.00019840381f;
  u = u * x + 0.0083333179f;
  return u * x + -0.16666666f;
}

/**
 * @description: f1 和 f2（多项式逼近）
 * @param {float} x
 * @return {*}
 */
float f2(float x) {
  float u = 1.7290616e-09f;
  u = u * x + -2.7093486e-07f;
  u = u * x + 2.4771643e-05f;
  u = u * x + -0.0013887906f;
  u = u * x + 0.041666519f;
  return u * x + -0.49999991f;
}

/**
 * @description: [核心函数]生成sin;
 * @param {float} x
 * @return {*}
 */
float fast_sin(float x) {
  int si = (int)(x * 0.31830988f);
  x = x - (float)si * Sg_PI;
  if (si & 1) {
    x = x > 0.0f ? x - Sg_PI : x + Sg_PI;
  }
  return x + x * x * x * f1(x * x);
}

/**
 * @description: [核心函数]生成cos;
 * @param {float} x
 * @return {*}
 */
float fast_cos(float x) {
  int si = (int)(x * 0.31830988f);
  x = x - (float)si * Sg_PI;
  if (si & 1) {
    x = x > 0.0f ? x - Sg_PI : x + Sg_PI;
  }
  return 1.0f + x * x * f2(x * x);
}

/**
 * @description: [核心函数]联合求解sin和cos;
 * @param {float} x
 * @param {float} *sin_x
 * @param {float} *cos_x
 * @return {*}
 */
void fast_sin_cos(float x, float *sin_x, float *cos_x) {
  int si = (int)(x * 0.31830988f);
  x = x - (float)si * Sg_PI;
  if (si & 1) {
    x = x > 0.0f ? x - Sg_PI : x + Sg_PI;
  }
  *sin_x = x + x * x * x * f1(x * x);
  *cos_x = 1.0f + x * x * f2(x * x);
}

