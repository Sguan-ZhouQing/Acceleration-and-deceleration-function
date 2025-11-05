#ifndef __MOTOR_PARAMETERS_H
#define __MOTOR_PARAMETERS_H

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
// 常见数学包含
#define Sg_Max(a, b) (((a) > (b)) ? (a) : (b))
#define Sg_Min(a, b) (((a) < (b)) ? (a) : (b))
#define Sg_Limit(val, max, min) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define Sg_PI 3.14159265358979323846f
#define rad60 1.047197551196598f
#define SQRT3 1.73205080756887729353f

// 电机参数包含




#endif // MOTOR_PARAMETERS_H
