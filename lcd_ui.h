#ifndef __LCD_UI_H
#define __LCD_UI_H

#include "main.h"
#include "lcd_drv.h"

// 屏幕布局定义
#define UI_GRAPH_X_START      0
#define UI_GRAPH_Y_START      0
#define UI_GRAPH_WIDTH        (LCD_W * 3 / 4)    // 左侧3/4区域用于图表
#define UI_GRAPH_HEIGHT       LCD_H

#define UI_PARAM_X_START      UI_GRAPH_WIDTH     // 右侧1/4区域备用
#define UI_PARAM_Y_START      0
#define UI_PARAM_WIDTH        (LCD_W - UI_GRAPH_WIDTH)
#define UI_PARAM_HEIGHT       LCD_H

// 图表区域定义
#define GRAPH_MARGIN_LEFT     20      // 左侧留出Y轴标签空间
#define GRAPH_MARGIN_RIGHT    5
#define GRAPH_MARGIN_TOP      5
#define GRAPH_MARGIN_BOTTOM   15      // 底部留出X轴标签空间

#define GRAPH_X_START         (UI_GRAPH_X_START + GRAPH_MARGIN_LEFT)
#define GRAPH_Y_START         (UI_GRAPH_Y_START + GRAPH_MARGIN_TOP)
#define GRAPH_WIDTH           (UI_GRAPH_WIDTH - GRAPH_MARGIN_LEFT - GRAPH_MARGIN_RIGHT)
#define GRAPH_HEIGHT          (UI_GRAPH_HEIGHT - GRAPH_MARGIN_TOP - GRAPH_MARGIN_BOTTOM)

// 时间轴定义 (5秒)
#define TIME_WINDOW_SECONDS   5
#define MAX_DATA_POINTS      GRAPH_WIDTH  // 每个像素点一个数据

// 字号定义
#define FONT_SIZE_SMALL       12

// 曲线结构体
typedef struct {
    float data_buffer[MAX_DATA_POINTS];  // 数据缓冲区
    uint16_t data_index;                 // 当前数据索引
    uint16_t color;                      // 曲线颜色
    float min_value;                     // 最小值
    float max_value;                     // 最大值
} GraphCurve_t;

// UI管理结构体
typedef struct {
    GraphCurve_t curves[3];             // 最多支持3条曲线
    uint8_t curve_count;                // 当前曲线数量
    uint16_t bg_color;                  // 背景颜色
    uint16_t axis_color;                // 坐标轴颜色
    uint16_t grid_color;                // 网格颜色
} LCD_UI_t;

// 初级函数声明（基础显示）
void LCD_UI_DrawCoordinateSystem(void);
void LCD_UI_AddCurve(uint8_t curve_id, uint16_t color);
void LCD_UI_UpdateCurveData(uint8_t curve_id, float value);
void LCD_UI_RefreshGraph(void);
void LCD_UI_UpdateDisplay(void);
void LCD_ShowChannelValues(uint8_t channel, uint16_t color, float value);
// 高级函数声明（界面UI）
void LCD_MainUI_0(uint16_t color_ch0, uint16_t color_ch1, uint16_t color_ch2, float CH0, float CH1, float CH2,uint8_t UseCurve_Flag);
void LCD_GlobalUI_Tick(void);

#endif

