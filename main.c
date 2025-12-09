/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "nRF24L01.h"
#include "Key.h"
#include "OLED.h"

#include "nRF_printf.h"
#include "Timer.h"
extern uint32_t ADC_InjectedValues[4];
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


int float_to_ascii_with_length_ex(float num, uint8_t *data, size_t data_size, int decimal_places) {
    char buffer[32];
    char format[8];
    
    // 根据小数位数构建格式字符串
    if (decimal_places > 0) {
        snprintf(format, sizeof(format), "%%.%df", decimal_places);
        snprintf(buffer, sizeof(buffer), format, num);
    } else if (decimal_places == 0) {
        // 四舍五入到整数
        snprintf(buffer, sizeof(buffer), "%.0f", num);
    } else {
        // 自动模式，使用%g
        snprintf(buffer, sizeof(buffer), "%g", num);
    }
    
    int len = (int)strlen(buffer);
    
    // 检查目标数组是否足够大
    if (data_size < (size_t)(len + 1)) {
        return -1;
    }
    
    // 第一个字节存储字符串的长度
    if (len > 255) {
        return -1;
    }
    
    data[0] = (uint8_t)len;
    
    // 复制字符串
    memcpy(&data[1], buffer, len);
    
    return len + 1;
}

int floats_to_ascii_with_length_ex(float *nums, int num_count, uint8_t *data, size_t data_size, int decimal_places) {
    char buffer[256] = {0};  // 增大缓冲区以容纳多个数字
    char format[8];
    char temp[32];
    
    if (num_count <= 0 || nums == NULL || data == NULL) {
        return -1;
    }
    
    // 构建格式字符串
    if (decimal_places > 0) {
        snprintf(format, sizeof(format), "%%.%df", decimal_places);
    } else if (decimal_places == 0) {
        snprintf(format, sizeof(format), "%%.0f");
    } else {
        // 对于自动模式，我们需要单独处理每个数字
    }
    
    // 构建完整的字符串，用逗号分隔
    for (int i = 0; i < num_count; i++) {
        if (i > 0) {
            strcat(buffer, ",");
        }
        
        if (decimal_places >= 0) {
            // 指定小数位数或整数模式
            snprintf(temp, sizeof(temp), format, nums[i]);
        } else {
            // 自动模式
            snprintf(temp, sizeof(temp), "%g", nums[i]);
        }
        
        // 检查缓冲区是否足够
        if (strlen(buffer) + strlen(temp) + 1 >= sizeof(buffer)) {
            return -1;  // 缓冲区溢出
        }
        
        strcat(buffer, temp);
    }
    
    int len = (int)strlen(buffer);
    
    // 检查目标数组是否足够大
    if (data_size < (size_t)(len + 1)) {
        return -1;
    }
    
    // 第一个字节存储字符串的长度
    if (len > 255) {
        return -1;
    }
    
    data[0] = (uint8_t)len;
    
    // 复制字符串
    memcpy(&data[1], buffer, len);
    
    return len + 1;  // 返回总字节数（长度字节 + 字符串）
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(300);
  OLED_Init();
  NRF24L01_Init();
  HAL_TIM_Base_Start_IT(&htim1);

  uint8_t Buff_Tx1[32] = {1,0x01};
  uint8_t Buff_Tx2[32] = {1,0x02};
  uint8_t Buff_Tx3[32] = {1,0x03};
  uint8_t Buff_Tx4[32] = {1,0x04};
  uint8_t Buff_Tx5[32] = {1,0x05};
  uint8_t Buff_Tx6[32] = {1,0x06};
  uint8_t Buff_Tx7[32] = {1,0x07};
  uint8_t Buff_Tx8[32] = {1,0x08};
  uint8_t Buff_Tx9[32] = {5,0x09,0x08,0x07,0x06,0x05};
  uint8_t Buff_Tx10[32] = {10,0x10,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t num_a = 0;
  uint8_t num_b = 0;
  uint8_t num_c = 0;
  uint8_t num_d = 0;
  float sensor_data[] = {12.2f, 13.3f};
  float num3 = 3.14159f;
  uint8_t data1[32];
  

  float numbers[] = {58.45f, 13.32f, 16.82f};
  uint8_t data3[32] = {0};

  while (1)
  {
    OLED_ShowString(10,0,"Sguan_nRF24",OLED_8X16);
    OLED_ShowSignedNum(0,20,(ADC_InjectedValues[0]*200/4096 - 100),3,OLED_8X16);
    OLED_ShowSignedNum(64,20,(ADC_InjectedValues[1]*200/4096 - 100),3,OLED_8X16);
    OLED_Update();
    uint8_t Key_Count = Key_GetNum();
    switch (Key_Count)
    {
    case 1:
      // OLED_ShowString(0,23,"K1",OLED_8X16);
      // NRF24L01_SendBuf(Buff_Tx1);
      // num_a += 1;
    
      // 方法1：使用辅助函数（内部创建缓冲区）
      int result = floats_to_ascii_with_length_ex(
          numbers, 
          3,          // 数字个数
          data3, 
          sizeof(data3), 
          2           // 保留2位小数
      );
      NRF24L01_SendBuf(data3);
      break;
    case 2:
      OLED_ShowString(0,23,"K2",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx2);
      num_b += 1;
      break;
    case 3:
      OLED_ShowString(0,23,"K3",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx3);      
      num_c += 1;
      break;
    case 4:
      OLED_ShowString(0,23,"K4",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx4);      
      num_d += 1;
      break;
    case 5:
      OLED_ShowString(0,23,"K5",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx5);      
      num_a -= 1;
      break;
    case 6:
      OLED_ShowString(0,23,"K6",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx6);   
      num_b -= 1;   
      break;
    case 7:
      OLED_ShowString(0,23,"K7",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx7);      
      num_c -= 1;
      break;
    case 8:
      OLED_ShowString(0,23,"K8",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx8);      
      num_d -= 1;
      break;
    case 9:
      OLED_ShowString(0,23,"K9",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx9);      
      num_a += 1;
      num_b += 1;
      num_c += 1;
      num_d += 1;
      break;
    case 10:
      OLED_ShowString(0,23,"K10",OLED_8X16);
      NRF24L01_SendBuf(Buff_Tx10);
      num_a -= 1;
      num_b -= 1;
      num_c -= 1;
      num_d -= 1; 
      break;
    
    default:
      break;
    }
    OLED_ShowSignedNum(0,45,num_a,2,OLED_8X16);
    OLED_ShowSignedNum(30,45,num_b,2,OLED_8X16);
    OLED_ShowSignedNum(60,45,num_c,2,OLED_8X16);
    OLED_ShowSignedNum(90,45,num_d,2,OLED_8X16);
    OLED_Update();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
