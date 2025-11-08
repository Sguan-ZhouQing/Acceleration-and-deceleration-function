#ifndef __LCD_UI_H
#define __LCD_UI_H

#include "main.h"
#include "lcd_drv.h"

// 屏幕布局定义
#define UI_GRAPH_X_START      0
#define UI_GRAPH_Y_START      0
#define UI_GRAPH_WIDTH        (LCD_W * 3 / 4)    // 左侧3/4区域用于图表
#define UI_GRAPH_HEIGHT       LCD_H

#define UI_PARAM_X_START      UI_GRAPH_WIDTH     // 右侧1/4区域用于参数显示
#define UI_PARAM_Y_START      0
#define UI_PARAM_WIDTH        (LCD_W - UI_GRAPH_WIDTH)
#define UI_PARAM_HEIGHT       LCD_H

// 图表区域定义
#define GRAPH_MARGIN_LEFT     25      // 左侧留出Y轴标签空间 (缩小)
#define GRAPH_MARGIN_RIGHT    5
#define GRAPH_MARGIN_TOP      5
#define GRAPH_MARGIN_BOTTOM   15      // 底部留出X轴标签空间 (缩小)

#define GRAPH_X_START         (UI_GRAPH_X_START + GRAPH_MARGIN_LEFT)
#define GRAPH_Y_START         (UI_GRAPH_Y_START + GRAPH_MARGIN_TOP)
#define GRAPH_WIDTH           (UI_GRAPH_WIDTH - GRAPH_MARGIN_LEFT - GRAPH_MARGIN_RIGHT)
#define GRAPH_HEIGHT          (UI_GRAPH_HEIGHT - GRAPH_MARGIN_TOP - GRAPH_MARGIN_BOTTOM)

// 时间轴定义 (5秒)
#define TIME_WINDOW_SECONDS   5
#define MAX_DATA_POINTS       (GRAPH_WIDTH)  // 每个像素点一个数据

// 参数显示区域定义 (去掉标签后可以更紧凑)
#define PARAM_ITEM_HEIGHT     20      // 每个参数项的高度
#define PARAM_ITEM_MARGIN     2       // 参数项之间的间距

// 字号定义 (0.7倍缩放)
#define FONT_SIZE_SMALL       12       // 12 * 0.7 ≈ 8

// 曲线结构体
typedef struct {
    float *data_buffer;           // 数据缓冲区
    uint16_t data_index;          // 当前数据索引
    uint16_t color;               // 曲线颜色
    char name[16];               // 曲线名称
    float min_value;             // 最小值
    float max_value;             // 最大值
} GraphCurve_t;

// UI管理结构体
typedef struct {
    GraphCurve_t curves[3];      // 最多支持3条曲线
    uint8_t curve_count;         // 当前曲线数量
    uint32_t last_update_time;   // 上次更新时间
    uint16_t bg_color;           // 背景颜色
    uint16_t axis_color;         // 坐标轴颜色
    uint16_t grid_color;         // 网格颜色
} LCD_UI_t;

// 函数声明
void LCD_UI_Init(void);
void LCD_UI_DrawCoordinateSystem(float y_min, float y_max);
void LCD_UI_AddCurve(uint8_t curve_id, const char* name, uint16_t color, float min_val, float max_val);
void LCD_UI_UpdateCurveData(uint8_t curve_id, float value);
void LCD_UI_RefreshGraph(void);
void LCD_UI_DisplayParameter(uint8_t row, float value, uint16_t color);  // 修改函数声明，去掉label参数
void LCD_UI_ClearParameterArea(void);
void LCD_UI_UpdateDisplay(void);
LCD_UI_t* LCD_UI_GetHandle(void);

#endif
