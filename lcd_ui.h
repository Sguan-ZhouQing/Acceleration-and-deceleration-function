#ifndef __LCD_UI_H
#define __LCD_UI_H

#include "main.h"
#include "lcd_drv.h"

// 图表区域定义
#define CHART_X_START 0
#define CHART_Y_START 0
#define CHART_WIDTH (LCD_W * 3 / 4)    // 屏幕3/4宽度
#define CHART_HEIGHT LCD_H

// 坐标轴范围
#define Y_AXIS_MIN (-30)
#define Y_AXIS_MAX 250
#define X_AXIS_MIN 0
#define X_AXIS_MAX 5

// 曲线最大数量
#define MAX_CURVES 5

// 曲线数据结构
typedef struct {
    float data[CHART_WIDTH];  // 存储每个X位置对应的Y值
    uint16_t color;           // 曲线颜色
    uint8_t active;           // 曲线是否激活
} Curve_t;

// 图表管理器
typedef struct {
    Curve_t curves[MAX_CURVES];
    uint8_t curve_count;
    uint16_t current_x;       // 当前绘制位置（像素）
    float time_elapsed;       // 已过去的时间（秒）
} ChartManager_t;

// 函数声明
void LCD_UI_Init(void);
void LCD_UI_DrawCoordinateSystem(void);
void LCD_UI_AddCurve(uint16_t color);
void LCD_UI_UpdateCurveData(uint8_t curve_index, float y_value);
void LCD_UI_AdvanceTime(float time_step);
void LCD_UI_RefreshChart(void);
void LCD_UI_ClearAllCurves(void);
// 函数声明
void LCD_ShowChannelValues(uint8_t channel, uint16_t color, float value);

#endif

