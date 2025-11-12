#include "lcd_ui.h"
#include "string.h"
// 外部函数声明
#include "Key.h"
#include "Encoder.h"
#include "foc.h"
#include "motor_pid.h"
#include "Current.h"

extern const unsigned char gImage_font_Assassin[25608];
extern float real_speed;
extern PID_STRUCT SguanVal;
extern volatile uint32_t ADC_InjectedValues[4];

static LCD_UI_t lcd_ui;
// 简单的浮点数转字符串函数
static void float_to_str(float value, char* buffer, int precision) {
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
    
    if (integer_part == 0) {
        *buffer++ = '0';
    } else {
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
    }
    
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
static void int_to_str(int value, char* buffer) {
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
static void LCD_UI_Init(void) {
    // 初始化UI管理结构体
    memset(&lcd_ui, 0, sizeof(LCD_UI_t));
    lcd_ui.bg_color = BLACK;
    lcd_ui.axis_color = WHITE;
    lcd_ui.grid_color = GRAY;
}


/**
  * @brief  绘制坐标系
  */
void LCD_UI_DrawCoordinateSystem(void) {
    uint16_t x, y;
    char temp[8];
    
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
    
    // 绘制Y轴刻度标签 (-30 到 250)
    const float y_values[] = {-30, 30, 90, 150, 210, 250};
    for (int i = 0; i <= 5; i++) {
        y = GRAPH_Y_START + GRAPH_HEIGHT - (i * GRAPH_HEIGHT / 5);
        
        // 使用自定义转换函数
        float_to_str(y_values[i], temp, 0);
        LCD_ShowString(GRAPH_X_START - 18, y - 6, (uint8_t*)temp, lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
        
        // 绘制刻度线
        LCD_DrawLine(GRAPH_X_START - 2, y, GRAPH_X_START, y, lcd_ui.axis_color);
    }
    
    // 绘制X轴刻度标签 (时间 0-5秒)
    for (int i = 0; i <= 5; i++) {
        x = GRAPH_X_START + (i * GRAPH_WIDTH / 5);
        
        // 使用自定义转换函数
        int_to_str(i, temp);
        LCD_ShowString(x - 3, GRAPH_Y_START + GRAPH_HEIGHT + 2, (uint8_t*)temp, lcd_ui.axis_color, lcd_ui.bg_color, FONT_SIZE_SMALL, 0);
        
        // 绘制刻度线
        LCD_DrawLine(x, GRAPH_Y_START + GRAPH_HEIGHT, x, GRAPH_Y_START + GRAPH_HEIGHT + 2, lcd_ui.axis_color);
    }
}

/**
  * @brief  添加曲线
  * @param  curve_id: 曲线ID (0-2)
  * @param  color: 曲线颜色
  */
void LCD_UI_AddCurve(uint8_t curve_id, uint16_t color) {
    if (curve_id >= 3) return;
    
    // 初始化曲线数据
    memset(lcd_ui.curves[curve_id].data_buffer, 0, sizeof(lcd_ui.curves[curve_id].data_buffer));
    lcd_ui.curves[curve_id].data_index = 0;
    lcd_ui.curves[curve_id].color = color;
    lcd_ui.curves[curve_id].min_value = -30.0f;
    lcd_ui.curves[curve_id].max_value = 250.0f;
    
    if (lcd_ui.curve_count <= curve_id) {
        lcd_ui.curve_count = curve_id + 1;
    }
}

/**
  * @brief  更新曲线数据
  * @param  curve_id: 曲线ID
  * @param  value: 新数据值
  */
void LCD_UI_UpdateCurveData(uint8_t curve_id, float value) {
    if (curve_id >= lcd_ui.curve_count) return;
    
    // 限制数据范围
    if (value < -20.0f) value = -20.0f;
    if (value > 250.0f) value = 250.0f;
    
    // 更新数据缓冲区
    lcd_ui.curves[curve_id].data_buffer[lcd_ui.curves[curve_id].data_index] = value;
    lcd_ui.curves[curve_id].data_index = (lcd_ui.curves[curve_id].data_index + 1) % MAX_DATA_POINTS;
}

/**
  * @brief  刷新图表显示
  */
void LCD_UI_RefreshGraph(void) {
    // 清空曲线显示区域 (保留坐标轴)
    LCD_Fill_Fast(GRAPH_X_START + 1, GRAPH_Y_START + 1,
                 GRAPH_X_START + GRAPH_WIDTH - 1,
                 GRAPH_Y_START + GRAPH_HEIGHT - 1,
                 lcd_ui.bg_color);
    
    // 绘制每条曲线
    for (uint8_t i = 0; i < lcd_ui.curve_count; i++) {
        uint16_t prev_x = 0, prev_y = 0;
        uint8_t first_point = 1;
        
        // 绘制曲线上的每个点
        for (uint16_t j = 0; j < MAX_DATA_POINTS; j++) {
            uint16_t buffer_index = (lcd_ui.curves[i].data_index + j) % MAX_DATA_POINTS;
            float value = lcd_ui.curves[i].data_buffer[buffer_index];
            
            // 计算屏幕坐标
            uint16_t x = GRAPH_X_START + j;
            uint16_t y = GRAPH_Y_START + GRAPH_HEIGHT - 
                        (uint16_t)((value - lcd_ui.curves[i].min_value) * GRAPH_HEIGHT / 
                                  (lcd_ui.curves[i].max_value - lcd_ui.curves[i].min_value));
            
            // 确保Y坐标在有效范围内
            if (y < GRAPH_Y_START) y = GRAPH_Y_START;
            if (y > GRAPH_Y_START + GRAPH_HEIGHT) y = GRAPH_Y_START + GRAPH_HEIGHT;
            
            if (!first_point) {
                // 绘制线段连接前后两个点
                LCD_DrawLine(prev_x, prev_y, x, y, lcd_ui.curves[i].color);
            }
            
            prev_x = x;
            prev_y = y;
            first_point = 0;
        }
    }
}

/**
  * @brief  更新整个显示
  */
void LCD_UI_UpdateDisplay(void) {
    LCD_UI_RefreshGraph();
}



/* 显示参数信息（重要）*/
void LCD_ShowChannelValues(uint8_t channel, uint16_t color, float value) {
    // 计算显示区域 - 屏幕右侧1/4
    static uint16_t display_x_start = LCD_W * 3 / 4;
    static uint16_t display_width = LCD_W / 4;
    // static uint16_t display_height = LCD_H;
    
    // 计算每个通道的显示区域高度
    static uint16_t channel_height = 26;
    
    // 计算每个通道的Y坐标起始位置
    static uint16_t channel_y_positions[3] = {
        4,                      // 通道0
        30,         // 通道1  
        56      // 通道2
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
    LCD_ShowSignedFloatNum(x_pos-2, y_pos + 4, value, 5, color, BLACK, 12);
}


void LCD_MainUI_0(uint16_t color_ch0, uint16_t color_ch1, uint16_t color_ch2, float CH0, float CH1, float CH2,uint8_t UseCurve_Flag) {
    static uint8_t Start_Flag = 0;
    if (!Start_Flag) {
        LCD_UI_Init();
        Start_Flag = 1;
        LCD_UI_AddCurve(0, color_ch0);
        LCD_UI_AddCurve(1, color_ch1);
        LCD_UI_AddCurve(2, color_ch2);
    }
    LCD_ShowChannelValues(0, color_ch0, CH0);    // 通道0显示红色
    LCD_ShowChannelValues(1, color_ch1, CH1);    // 通道1显示绿色  
    LCD_ShowChannelValues(2, color_ch2, CH2);    // 通道2显示蓝色
    if (UseCurve_Flag) {
        LCD_UI_UpdateCurveData(0, CH0);
        LCD_UI_UpdateCurveData(1, CH1);
        LCD_UI_UpdateCurveData(2, CH2);
        LCD_UI_UpdateDisplay();
    } 
}

// 主循环调用的屏幕UI刷新函数
void LCD_GlobalUI_Tick(void) {
    static int8_t UI_Num = 0;
    static int8_t UI_Num_Last = 0;
    uint8_t Key_Count = Key_GetNum();
    if (Key_Count == 1) {
      UI_Num += 1;
    }
    if (Key_Count == 2) {
      UI_Num -= 1;
    }
    UI_Num = (UI_Num > 2) ? 2 : ((UI_Num < 0) ? 0 : UI_Num);
    if (UI_Num != UI_Num_Last) {
        LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
        if (UI_Num == 1 || UI_Num == 2) {
            LCD_UI_DrawCoordinateSystem();
        }
    }
    UI_Num_Last = UI_Num;
    switch (UI_Num)
    {
    case 0:       
        LCD_ShowPicture(0,0,160,80,gImage_font_Assassin);
        break;
    case 1:
        LCD_MainUI_0(RED, GREEN, BLUE, real_speed, ADC_InjectedValues[3]*250/4096, SguanVal.Out*250, 0);
        break;
    case 2:
        LCD_MainUI_0(RED, GREEN, BLUE, real_speed, ADC_InjectedValues[3]*250/4096, SguanVal.Out*250, 1);
        break;
    
    default:
        break;
    }
    
}


