#ifndef __MODEL_PLL_H
#define __MODEL_PLL_H

#include "stm32g4xx_hal.h"

typedef struct
{	
	signed char    Dir;                  //反电动势计算方向 
	float Ts;	                //调用周期		

	float Ain;                  //输入A
	float Bin;	                //输入B

	float ThetaErr;		        //观测角度误差		
	float ThetaFore;		    //观测角度 单位：弧度
	float ThetaCompensate;      //补偿后的观测角度 单位：弧度
	float ETheta;               //补偿后的观测角度 0-3999

	float SinVal;               //正弦值
	float CosVal;               //余弦值

	float	Kp;        		    //锁相环KP
	float	Ki;          		//锁相环KI
	float PPart;	            //积分项
	float IPart;	            //积分项
		
	float WeFore;	            //观测电角速度（rad/s）	
	float WeForeLPF;         	//观测电角速度滤波值	
	float WeForeLPFFactor;  	//观测电角速度滤波系数	
}PLL_STRUCT;

void PLL_Calculate(PLL_STRUCT *p);


#endif // MODEL_PLL_H
