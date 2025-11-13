#ifndef __MODEL_SMO_H
#define __MODEL_SMO_H

#include "stm32g4xx_hal.h"

typedef struct
{  	
	float Ts;	                //调用周期	
	float Rs;                   //相电阻
	float Ld;                   //相电感
	float Gain;                 //滑膜观测器增益

	float Ialpha;               //α轴实际电流
	float Ibeta;                //β轴实际电流		

	float IalphaFore;           //α轴预测电流
	float IbetaFore;            //β轴预测电流

	float Ualpha;               //α轴实际电压
	float Ubeta;                //β轴实际电压	

	float EalphaFore;           //α轴预测反势
	float EalphaForeLPF;        //α轴预测反势滤波值	
	float EalphaForeLPFFactor;  //α轴预测反势滤波系数

	float EbetaFore;            //β轴预测反势
	float EbetaForeLPF;         //β轴预测反势滤波值
	float EbetaForeLPFFactor;   //β轴预测反势滤波系数
}SMO_STRUCT;


void SMO_Calculate(SMO_STRUCT *p);


#endif // MODEL_SMO_H
