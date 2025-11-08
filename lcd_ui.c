#include "lcd_ui.h"
#include "string.h"

static LCD_UI_t lcd_ui;
static char temp_str[32];

// 参数显示缓冲区 (简化，只存储数值和颜色)
static struct {
    float value;
    uint16_t color;
    uint8_t updated;
} param_display[3] = {0};

// 静态分配数据缓冲区，避免动态内存分配
static float curve_data_buffer[3][MAX_DATA_POINTS];

// 重写标准库函数，避免半主机依赖
__attribute__((used)) int _write(int file, char *ptr, int len)
{
    return len;
}

// 简单的浮点数转字符串函数，替代snprintf
static void float_to_str(float value, char* buffer, int precision)
{
    int integer_part = (int)value;
    float fractional_part = value - integer_part;
    
    // 处理负数
    if (value < 0) {
        *buffer++ = '-';
        integer_part = -integer_part;
        fractional_part = -fractional_part;
    }
    
    // 转换整数部分
    char int_str[16];
    char *p = int_str;
    int len = 0;
    
    do {
        *p++ = '0' + (integer_part % 10);
        integer_part /= 10;
        len++;
    } while (integer_part > 0);
    
    // 反转整数部分
    for (int i = 0; i < len; i++) {
        buffer[i] = int_str[len - 1 - i];
    }
    buffer += len;
    
    if (precision > 0) {
        *buffer++ = '.';
        
        // 转换小数部分
        for (int i = 0; i < precision; i++) {
            fractional_part *= 10;
            int digit = (int)fractional_part;
            *buffer++ = '0' + digit;
            fractional_part -= digit;
        }
    }
    
    *buffer = '\0';
}

// 简单的整数转字符串函数
static void int_to_str(int value, char* buffer)
{
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    
    char temp[16];
    char *p = temp;
    int len = 0;
    int is_negative = 0;
    
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    while (value > 0) {
        *p++ = '0' + (value % 10);
        value /= 10;
        len++;
    }
    
    char *dst = buffer;
    if (is_negative) {
        *dst++ = '-';
    }
    
    for (int i = 0; i < len; i++) {
        dst[i] = temp[len - 1 - i];
    }
    dst[len] = '\0';
}

/**
  * @brief  UI初始化
  */
void LCD_UI_Init(void)
{
    HAL_Delay(500);
    LCD_Init();
    // 初始化UI管理结构体
    memset(&lcd_ui, 0, sizeof(LCD_UI_t));
    lcd_ui.bg_color = BLACK;
    lcd_ui.axis_color = WHITE;
    lcd_ui.grid_color = GRAY;
    lcd_ui.last_update_time = HAL_GetTick();
    
    // 初始化参数显示区
    memset(param_display, 0, sizeof(param_display));
    
    // 初始化曲线数据缓冲区
    for (int i = 0; i < 3; i++) {
        memset(curve_data_buffer[i], 0, sizeof(curve_data_buffer[i]));
    }
    
    // 清屏
    LCD_Fill_Fast(0, 0, LCD_W, LCD_H, lcd_ui.bg_color);
    
    // 绘制初始坐标系 (默认速度范围: -30 到 250)
    LCD_UI_DrawCoordinateSystem(-30.0f, 250.0f);
    
    // 绘制参数显示区域边框
    LCD_DrawLine(UI_PARAM_X_START, 0, UI_PARAM_X_START, LCD_H, lcd_ui.axis_color);
}

/**
  * @brief  绘制坐标系
  * @param  y_min: Y轴最小值
  * @param  y_max: Y轴最大值
  */
void LCD_UI_DrawCoordinateSystem(float y_min, float y_max)
{
    uint16_t x, y;
    char temp[16];
    
    // 清空图表区域
    LCD_Fill_Fast(UI_GRAPH_X_START, UI_GRAPH_Y_START, 
                 UI_GRAPH_X_START + UI_GRAPH_WIDTH, 
                 UI_GRAPH_Y_START + UI_GRAPH_HEIGHT, 
                 lcd_ui.bg_color);
    
    // 绘制坐标轴
    // X轴 (底部)
    LCD_DrawLine(GRAPH_X_START, GRAPH_Y_START + GRAPH_HEIGHT,
                 GRAPH_X_START + GRAPH_WIDTH, GRAPH_Y_START + GRAPH_HEIGHT,
                 lcd_ui.axis_color);
    
    // Y轴 (左侧)
    LCD_DrawLine(GRAPH_X_START, GRAPH_Y_START,
                 GRAPH_X_START, GRAPH_Y_START + GRAPH_HEIGHT,
                 lcd_ui.axis_color);
    
    // 绘制网格线
    // 水平网格线 (Y轴)
    for (int i = 1; i < 5; i++) {
        y = GRAPH_Y_START + (i * GRAPH_HEIGHT / 5);
        LCD_DrawLine(GRAPH_X_START, y, GRAPH_X_START + GRAPH_WIDTH, y, lcd_ui.grid_color);
    }
    
    // 垂直网格线 (X轴 - 时间轴)
    for (int i = 1; i < 5; i++) {
        x = GRAPH_X_START + (i * GRAPH_WIDTH / 5);
        LCD_DrawLine(x, GRAPH_Y_START, x, GRAPH_Y_START + GRAPH_HEIGHT, lcd_ui.grid_color);
    }
    
    // 绘制Y轴刻度标签 - 使用小字号
    for (int i = 0; i <= 5; i++) {
        y = GRAPH_Y_START + GRAPH_HEIGHT - (i * GRAPH_HEIGHT / 5);
        float value = y_min + (y_max - y_min) * i / 5;
        
        // 使用自定义转换函数替代snprintf
        float_to_str(value, temp, 0);
        LCD_ShowString(GRAPH_X_START - 22, y - 4, (uint8_t*)temp, lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
        
        // 绘制刻度线
        LCD_DrawLine(GRAPH_X_START - 2, y, GRAPH_X_START, y, lcd_ui.axis_color);
    }
    
    // 绘制X轴刻度标签 (时间) - 使用小字号
    for (int i = 0; i <= 5; i++) {
        x = GRAPH_X_START + (i * GRAPH_WIDTH / 5);
        int time_sec = i;
        
        // 使用自定义转换函数
        int_to_str(time_sec, temp);
        strcat(temp, "");
        LCD_ShowString(x - 6, GRAPH_Y_START + GRAPH_HEIGHT + 1, (uint8_t*)temp, lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
        
        // 绘制刻度线
        LCD_DrawLine(x, GRAPH_Y_START + GRAPH_HEIGHT, x, GRAPH_Y_START + GRAPH_HEIGHT + 2, lcd_ui.axis_color);
    }
    
    // 绘制坐标轴标签 - 使用小字号
    LCD_ShowString(GRAPH_X_START + GRAPH_WIDTH/2 - 10, GRAPH_Y_START + GRAPH_HEIGHT + 1, 
                   (uint8_t*)"", lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
    
    // Y轴标签 (垂直显示) - 使用小字号
    LCD_ShowString(3, GRAPH_Y_START + GRAPH_HEIGHT/2 - 20, 
                   (uint8_t*)"", lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
}

/**
  * @brief  添加曲线
  * @param  curve_id: 曲线ID (0-2)
  * @param  name: 曲线名称
  * @param  color: 曲线颜色
  * @param  min_val: 最小值
  * @param  max_val: 最大值
  */
void LCD_UI_AddCurve(uint8_t curve_id, const char* name, uint16_t color, float min_val, float max_val)
{
    if (curve_id >= 3) return;
    
    // 使用静态分配的数据缓冲区
    lcd_ui.curves[curve_id].data_buffer = curve_data_buffer[curve_id];
    lcd_ui.curves[curve_id].data_index = 0;
    lcd_ui.curves[curve_id].color = color;
    lcd_ui.curves[curve_id].min_value = min_val;
    lcd_ui.curves[curve_id].max_value = max_val;
    strncpy(lcd_ui.curves[curve_id].name, name, sizeof(lcd_ui.curves[curve_id].name) - 1);
    
    if (lcd_ui.curve_count <= curve_id) {
        lcd_ui.curve_count = curve_id + 1;
    }
}

/**
  * @brief  更新曲线数据
  * @param  curve_id: 曲线ID
  * @param  value: 新数据值
  */
void LCD_UI_UpdateCurveData(uint8_t curve_id, float value)
{
    if (curve_id >= lcd_ui.curve_count) return;
    if (lcd_ui.curves[curve_id].data_buffer == NULL) return;
    
    // 更新数据缓冲区
    lcd_ui.curves[curve_id].data_buffer[lcd_ui.curves[curve_id].data_index] = value;
    lcd_ui.curves[curve_id].data_index = (lcd_ui.curves[curve_id].data_index + 1) % MAX_DATA_POINTS;
}

/**
  * @brief  刷新图表显示
  */
void LCD_UI_RefreshGraph(void)
{
    // 清空曲线显示区域 (保留坐标轴)
    LCD_Fill_Fast(GRAPH_X_START + 1, GRAPH_Y_START + 1,
                 GRAPH_X_START + GRAPH_WIDTH - 1,
                 GRAPH_Y_START + GRAPH_HEIGHT - 1,
                 lcd_ui.bg_color);
    
    // 绘制每条曲线
    for (uint8_t i = 0; i < lcd_ui.curve_count; i++) {
        if (lcd_ui.curves[i].data_buffer == NULL) continue;
        
        uint16_t prev_x = 0, prev_y = 0;
        uint8_t first_point = 1;
        uint8_t prev_point_valid = 0;  // 标记前一个点是否有效
        
        // 绘制曲线上的每个点
        for (uint16_t j = 0; j < MAX_DATA_POINTS; j++) {
            uint16_t buffer_index = (lcd_ui.curves[i].data_index + j) % MAX_DATA_POINTS;
            float value = lcd_ui.curves[i].data_buffer[buffer_index];
            
            // 计算屏幕坐标
            uint16_t x = GRAPH_X_START + j;
            
            // 改进的坐标计算：确保值在有效范围内
            float normalized_value = value;
            if (normalized_value < lcd_ui.curves[i].min_value) {
                normalized_value = lcd_ui.curves[i].min_value;
            }
            if (normalized_value > lcd_ui.curves[i].max_value) {
                normalized_value = lcd_ui.curves[i].max_value;
            }
            
            uint16_t y = GRAPH_Y_START + GRAPH_HEIGHT - 
                        (uint16_t)((normalized_value - lcd_ui.curves[i].min_value) * GRAPH_HEIGHT / 
                                  (lcd_ui.curves[i].max_value - lcd_ui.curves[i].min_value));
            
            // 确保Y坐标在有效范围内
            if (y < GRAPH_Y_START) {
                y = GRAPH_Y_START;
            }
            if (y > GRAPH_Y_START + GRAPH_HEIGHT) {
                y = GRAPH_Y_START + GRAPH_HEIGHT;
            }
            
            // 检查当前点是否在有效区域内
            uint8_t current_point_valid = 1;
            
            // 改进：只有当值在有效范围内时才认为是有效点
            if (value < lcd_ui.curves[i].min_value || value > lcd_ui.curves[i].max_value) {
                current_point_valid = 0;
            }
            
            if (!first_point && j > 0) {
                // 只有当前后两个点都有效时才绘制线段
                if (prev_point_valid && current_point_valid) {
                    // 改进：检查两点是否都在图表区域内
                    if (!(prev_y == GRAPH_Y_START && y == GRAPH_Y_START) &&  // 不在顶部边界
                        !(prev_y == GRAPH_Y_START + GRAPH_HEIGHT && y == GRAPH_Y_START + GRAPH_HEIGHT)) {  // 不在底部边界
                        LCD_DrawLine(prev_x, prev_y, x, y, lcd_ui.curves[i].color);
                    } else {
                        // 如果两点都在边界上，只绘制点
                        LCD_DrawPoint(x, y, lcd_ui.curves[i].color);
                    }
                } else {
                    // 如果当前点或前一个点无效，只绘制单独的点（如果当前点有效）
                    if (current_point_valid) {
                        LCD_DrawPoint(x, y, lcd_ui.curves[i].color);
                    }
                }
            } else {
                // 绘制第一个点（如果有效）
                if (current_point_valid) {
                    LCD_DrawPoint(x, y, lcd_ui.curves[i].color);
                }
            }
            
            prev_x = x;
            prev_y = y;
            prev_point_valid = current_point_valid;
            first_point = 0;
        }
    }
}

/**
  * @brief  显示参数 (简化版本，只显示数值)
  * @param  row: 行号 (0-2)
  * @param  value: 参数值
  * @param  color: 显示颜色
  */
void LCD_UI_DisplayParameter(uint8_t row, float value, uint16_t color)
{
    if (row >= 3) return;
    
    // 更新参数缓冲区
    param_display[row].value = value;
    param_display[row].color = color;
    param_display[row].updated = 1;
}

/**
  * @brief  清除参数显示区域
  */
void LCD_UI_ClearParameterArea(void)
{
    LCD_Fill_Fast(UI_PARAM_X_START + 1, UI_PARAM_Y_START + 1,
                 UI_PARAM_X_START + UI_PARAM_WIDTH - 1,
                 UI_PARAM_Y_START + UI_PARAM_HEIGHT - 1,
                 lcd_ui.bg_color);
}

/**
  * @brief  更新整个显示
  */
void LCD_UI_UpdateDisplay(void)
{
    // 刷新图表
    LCD_UI_RefreshGraph();
    
    // 更新参数显示
    for (uint8_t i = 0; i < 3; i++) {
        if (param_display[i].updated) {
            uint16_t y_pos = UI_PARAM_Y_START + 10 + i * PARAM_ITEM_HEIGHT;
            
            // 清除该行区域
            LCD_Fill(UI_PARAM_X_START + 2, y_pos - 6,
                     UI_PARAM_X_START + UI_PARAM_WIDTH - 2,
                     y_pos + 10,
                     lcd_ui.bg_color);
            
            // 只显示数值 - 使用小字号，居中显示
            float_to_str(param_display[i].value, temp_str, 2);
            
            // 计算文本居中位置
            uint16_t text_width = strlen(temp_str) * (FONT_SIZE_SMALL / 2);
            uint16_t x_pos = UI_PARAM_X_START + (UI_PARAM_WIDTH - text_width) / 2;
            
            LCD_ShowString(x_pos, y_pos, 
                          (uint8_t*)temp_str, 
                          param_display[i].color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
            
            param_display[i].updated = 0;
        }
    }
    
    lcd_ui.last_update_time = HAL_GetTick();
}

/**
  * @brief  获取UI状态
  * @return UI管理结构体指针
  */
LCD_UI_t* LCD_UI_GetHandle(void)
{
    return &lcd_ui;
}
