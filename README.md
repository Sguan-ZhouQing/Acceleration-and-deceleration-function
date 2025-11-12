# Acceleration-and-deceleration-function
T-shaped and S-shaped curve acceleration

/* USER CODE BEGIN 2 */

Sguan_FocInit();

UART_Init();
HAL_TIM_Base_Start_IT(&htim2);

HAL_TIM_Base_Start_IT(&htim1);

Speed_PidInit();

LCD_Init();

/* USER CODE END 2 */

/* Infinite loop */

/* USER CODE BEGIN WHILE */

while (1)

{ 
  LCD_GlobalUI_Tick();
  
  printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%d,%d,%.5f,%.5f,%.5f,%d,%d\n",real_speed,Adjustable_Data,my_id,my_iq,my_alpha,my_beta,temp_IA,temp_IC,mysin,mycos,SguanVal.Out,ADC_InjectedValues[0],ADC_InjectedValues[3]);
  
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
}
