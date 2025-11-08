# Acceleration-and-deceleration-function
T-shaped and S-shaped curve acceleration

LCD_UI_Init();
LCD_UI_AddCurve(0, "", RED, -30.0f, 250.0f);
LCD_UI_AddCurve(1, "", YELLOW, -30.0f, 250.0f);
LCD_UI_AddCurve(2, "", BLUE, -30.0f, 250.0f);
while (1)
{ 
  LCD_UI_UpdateCurveData(0, ADC_Data.Potentiometer*75.0f);          // 目标速度
  LCD_UI_UpdateCurveData(1, real_speed);                            // 真实速度
  LCD_UI_UpdateCurveData(2, SguanVal.Out*250);                      // Q轴电压
  LCD_UI_DisplayParameter(0, ADC_Data.Potentiometer*75.0f, RED);
  LCD_UI_DisplayParameter(1, real_speed, YELLOW);
  LCD_UI_DisplayParameter(2, SguanVal.Out, BLUE);
  LCD_UI_UpdateDisplay();
}
