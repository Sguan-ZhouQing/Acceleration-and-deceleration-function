#include "lcd_ui.h"

static ChartManager_t chart_manager;

// 初始化图表UI
void LCD_UI_Init(void)
{
    // 初始化图表管理器
    chart_manager.curve_count = 0;
    chart_manager.current_x = 0;
    chart_manager.time_elapsed = 0.0f;
    
    // 清空所有曲线数据
    for(int i = 0; i < MAX_CURVES; i++) {
        chart_manager.curves[i].active = 0;
        for(int j = 0; j < CHART_WIDTH; j++) {
            chart_manager.curves[i].data[j] = Y_AXIS_MIN; // 初始化为最小值
        }
    }
    
    // 绘制坐标系
    LCD_UI_DrawCoordinateSystem();
}

// 绘制坐标系
void LCD_UI_DrawCoordinateSystem(void)
{
    // 清空图表区域
    LCD_Fill_Fast(CHART_X_START, CHART_Y_START, 
                 CHART_X_START + CHART_WIDTH, 
                 CHART_Y_START + CHART_HEIGHT, 
                 BLACK);
    
    // 绘制坐标轴（白色）
    // Y轴
    LCD_DrawLine(CHART_X_START + 20, CHART_Y_START + 10, 
                CHART_X_START + 20, CHART_Y_START + CHART_HEIGHT - 10, 
                WHITE);
    // X轴
    LCD_DrawLine(CHART_X_START + 20, CHART_Y_START + CHART_HEIGHT - 10, 
                CHART_X_START + CHART_WIDTH - 10, CHART_Y_START + CHART_HEIGHT - 10, 
                WHITE);
    
    // Y轴刻度标记
    int y_ticks[] = {-30, 0, 50, 100, 150, 200, 250};
    int y_pixel_range = CHART_HEIGHT - 20; // 减去上下边距
    
    for(int i = 0; i < sizeof(y_ticks)/sizeof(y_ticks[0]); i++) {
        int y_pixel = CHART_Y_START + CHART_HEIGHT - 10 - 
                     ((y_ticks[i] - Y_AXIS_MIN) * y_pixel_range / (Y_AXIS_MAX - Y_AXIS_MIN));
        
        // 绘制刻度线
        LCD_DrawLine(CHART_X_START + 18, y_pixel, 
                    CHART_X_START + 22, y_pixel, 
                    WHITE);
        
        // 显示刻度值
        char num_str[8];
        if(y_ticks[i] >= 0) {
            sprintf(num_str, "%d", y_ticks[i]);
        } else {
            sprintf(num_str, "-%d", -y_ticks[i]);
        }
        
        LCD_ShowString(CHART_X_START + 2, y_pixel - 6, (uint8_t*)num_str, WHITE, BLACK, 12, 0);
    }
    
    // X轴刻度标记
    for(int i = 0; i <= 5; i++) {
        int x_pixel = CHART_X_START + 20 + (i * (CHART_WIDTH - 30) / 5);
        
        // 绘制刻度线
        LCD_DrawLine(x_pixel, CHART_Y_START + CHART_HEIGHT - 12, 
                    x_pixel, CHART_Y_START + CHART_HEIGHT - 8, 
                    WHITE);
        
        // 显示刻度值
        char num_str[4];
        sprintf(num_str, "%d", i);
        LCD_ShowString(x_pixel - 3, CHART_Y_START + CHART_HEIGHT - 20, (uint8_t*)num_str, WHITE, BLACK, 12, 0);
    }
}

// 添加新曲线
void LCD_UI_AddCurve(uint16_t color)
{
    if(chart_manager.curve_count < MAX_CURVES) {
        int index = chart_manager.curve_count;
        chart_manager.curves[index].color = color;
        chart_manager.curves[index].active = 1;
        
        // 初始化曲线数据
        for(int i = 0; i < CHART_WIDTH; i++) {
            chart_manager.curves[index].data[i] = Y_AXIS_MIN;
        }
        
        chart_manager.curve_count++;
    }
}

// 更新曲线数据
void LCD_UI_UpdateCurveData(uint8_t curve_index, float y_value)
{
    if(curve_index < chart_manager.curve_count && chart_manager.curves[curve_index].active) {
        // 限制Y值在显示范围内
        if(y_value < Y_AXIS_MIN) y_value = Y_AXIS_MIN;
        if(y_value > Y_AXIS_MAX) y_value = Y_AXIS_MAX;
        
        chart_manager.curves[curve_index].data[chart_manager.current_x] = y_value;
    }
}

// 推进时间（移动曲线）
void LCD_UI_AdvanceTime(float time_step)
{
    chart_manager.time_elapsed += time_step;
    
    // 如果超过5秒，整体左移数据
    if(chart_manager.time_elapsed > X_AXIS_MAX) {
        // 所有曲线数据左移一位
        for(int i = 0; i < chart_manager.curve_count; i++) {
            if(chart_manager.curves[i].active) {
                for(int j = 0; j < CHART_WIDTH - 1; j++) {
                    chart_manager.curves[i].data[j] = chart_manager.curves[i].data[j + 1];
                }
                // 新数据位置设为最小值
                chart_manager.curves[i].data[CHART_WIDTH - 1] = Y_AXIS_MIN;
            }
        }
        chart_manager.time_elapsed -= 1.0f; // 减去1秒
    } else {
        // 正常推进当前绘制位置
        chart_manager.current_x = (uint16_t)((chart_manager.time_elapsed / X_AXIS_MAX) * (CHART_WIDTH - 30));
        if(chart_manager.current_x >= CHART_WIDTH) {
            chart_manager.current_x = CHART_WIDTH - 1;
        }
    }
}

// 刷新图表显示
void LCD_UI_RefreshChart(void)
{
    // 只重绘曲线区域，不重绘坐标轴
    int chart_area_x_start = CHART_X_START + 21;
    int chart_area_y_start = CHART_Y_START + 11;
    int chart_area_width = CHART_WIDTH - 31;
    int chart_area_height = CHART_HEIGHT - 21;
    
    // 清空曲线区域
    LCD_Fill_Fast(chart_area_x_start, chart_area_y_start, 
                 chart_area_x_start + chart_area_width, 
                 chart_area_y_start + chart_area_height, 
                 BLACK);
    
    // 绘制所有激活的曲线
    for(int curve_idx = 0; curve_idx < chart_manager.curve_count; curve_idx++) {
        if(!chart_manager.curves[curve_idx].active) continue;
        
        uint16_t curve_color = chart_manager.curves[curve_idx].color;
        
        // 绘制曲线（连接相邻数据点）
        for(int x = 1; x < CHART_WIDTH; x++) {
            float y1 = chart_manager.curves[curve_idx].data[x - 1];
            float y2 = chart_manager.curves[curve_idx].data[x];
            
            // 跳过无效数据点
            if(y1 <= Y_AXIS_MIN && y2 <= Y_AXIS_MIN) continue;
            
            // 转换为像素坐标
            int x1_pixel = chart_area_x_start + (x - 1) * chart_area_width / (CHART_WIDTH - 1);
            int x2_pixel = chart_area_x_start + x * chart_area_width / (CHART_WIDTH - 1);
            
            int y1_pixel = chart_area_y_start + chart_area_height - 
                          ((y1 - Y_AXIS_MIN) * chart_area_height / (Y_AXIS_MAX - Y_AXIS_MIN));
            int y2_pixel = chart_area_y_start + chart_area_height - 
                          ((y2 - Y_AXIS_MIN) * chart_area_height / (Y_AXIS_MAX - Y_AXIS_MIN));
            
            // 绘制线段
            LCD_DrawLine(x1_pixel, y1_pixel, x2_pixel, y2_pixel, curve_color);
        }
    }
}

// 清除所有曲线
void LCD_UI_ClearAllCurves(void)
{
    for(int i = 0; i < MAX_CURVES; i++) {
        chart_manager.curves[i].active = 0;
        for(int j = 0; j < CHART_WIDTH; j++) {
            chart_manager.curves[i].data[j] = Y_AXIS_MIN;
        }
    }
    chart_manager.curve_count = 0;
    chart_manager.current_x = 0;
    chart_manager.time_elapsed = 0.0f;
    
    // 重绘坐标系
    LCD_UI_DrawCoordinateSystem();
}




/**
  ******************************************************************************
  * 函数名称: LCD_ShowChannelValues
  * 功能描述: 在屏幕右侧1/4区域竖排显示3个通道的浮点数
  * 输入参数: channel - 通道号(0-2)
  *           color   - 显示颜色
  *           value   - 浮点数值
  * 输出参数: 无
  ******************************************************************************
  */
void LCD_ShowChannelValues(uint8_t channel, uint16_t color, float value)
{
    // 计算显示区域 - 屏幕右侧1/4
    static uint16_t display_x_start = LCD_W * 3 / 4;
    static uint16_t display_width = LCD_W / 4;
    static uint16_t display_height = LCD_H;
    
    // 计算每个通道的显示区域高度
    static uint16_t channel_height = LCD_H / 3;
    
    // 计算每个通道的Y坐标起始位置
    static uint16_t channel_y_positions[3] = {
        0,                      // 通道0
        channel_height,         // 通道1  
        channel_height * 2      // 通道2
    };
    
    // 根据通道号确定显示位置
    uint16_t x_pos, y_pos;
    
    switch(channel) {
        case 0:
            x_pos = display_x_start;
            y_pos = channel_y_positions[0];
            break;
        case 1:
            x_pos = display_x_start; 
            y_pos = channel_y_positions[1];
            break;
        case 2:
            x_pos = display_x_start;
            y_pos = channel_y_positions[2];
            break;
        default:
            return; // 通道号错误，直接返回
    }
    
    // 清空该通道的显示区域
    LCD_Fill_Fast(x_pos, y_pos, x_pos + display_width, y_pos + channel_height, BLACK);
    
    // 显示浮点数（保留2位小数）
    LCD_ShowFloatNum1(x_pos + 2, y_pos + 4, value, 6, color, BLACK, 12);
}

