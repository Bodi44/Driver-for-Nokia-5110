/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "lcd5110.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
LCD5110_display lcd1;
LCD5110_display lcd2;

void wait_for_button1_pressed();

const unsigned char smile[] = { 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0,
		0xf8, 0x7c, 0x7c, 0x3c, 0x1e, 0x1e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x1f, 0x1e, 0x1e, 0x3c, 0x7c, 0x7c, 0xf8,
		0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xfe, 0xff,
		0xff, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0xf0, 0xfc, 0xfe, 0xfe, 0xfe,
		0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xfc, 0xfe, 0xfe, 0xfe, 0xfc,
		0xf0, 0x00, 0x00, 0x02, 0x03, 0x07, 0x0f, 0xff, 0xff, 0xfc, 0xf0, 0x00,
		0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x40, 0xc0, 0xc0, 0xc0,
		0xc3, 0xc7, 0xc7, 0xc3, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3,
		0xc7, 0xc7, 0xc7, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff, 0x03, 0x07, 0x3f, 0x7f, 0xff, 0xf8, 0xf0, 0xc0,
		0x80, 0x03, 0x07, 0x0f, 0x1f, 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0xff, 0xff,
		0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x07, 0x01, 0x80,
		0xe0, 0xf0, 0xf8, 0xff, 0x7f, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x03, 0x03, 0x07, 0x0f, 0x1f, 0x1e, 0x3e, 0x3c, 0x3c, 0x78, 0x78,
		0x78, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x78, 0x78, 0x78, 0x3c, 0x3c,
		0x3e, 0x1e, 0x1f, 0x0f, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 //
		};

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();

  /* USER CODE BEGIN 2 */
  lcd1.hw_conf.spi_handle = &hspi2;
  lcd1.hw_conf.spi_cs_pin =  LCD1_CS_Pin;
  lcd1.hw_conf.spi_cs_port = LCD1_CS_GPIO_Port;
  lcd1.hw_conf.rst_pin =  LCD1_RST_Pin;
  lcd1.hw_conf.rst_port = LCD1_RST_GPIO_Port;
  lcd1.hw_conf.dc_pin =  LCD1_DC_Pin;
  lcd1.hw_conf.dc_port = LCD1_DC_GPIO_Port;
  lcd1.def_scr = lcd5110_def_scr;
  LCD5110_init(&lcd1.hw_conf, LCD5110_NORMAL_MODE, 0x40, 2, 3);


  LCD5110_line(1, 1, 30, 30, BLACK, &lcd1);
  LCD5110_line(1, 1, 3, 30, BLACK, &lcd1);
  LCD5110_line(1, 1, 30, 3, BLACK, &lcd1);

  LCD5110_line(35, 35, 20, 20, BLACK, &lcd1);
  LCD5110_line(35, 35, 2, 20, BLACK, &lcd1);
  LCD5110_line(35, 35, 20, 2, BLACK, &lcd1);

  LCD5110_line(83, 47, 20, 20, BLACK, &lcd1);
  LCD5110_line(83, 1,  20, 20, BLACK, &lcd1);
  LCD5110_line(1,  47, 20, 20, BLACK, &lcd1);

  LCD5110_refresh(&lcd1);

  wait_for_button1_pressed();
  LCD5110_clear_scr(&lcd1);

  LCD5110_canvas_t win1={ {1, 8, 30, 20}, {2, 2}, 1}; //-V525
  LCD5110_canvas_t win2={ {35, 5, 30, 20}, {2, 2}, 1};
  LCD5110_canvas_t win3={ {10, 10, 70, 30}, {2, 2}, 1};


  LCD5110_win_draw_frame(1, &win1, &lcd1);
  LCD5110_win_draw_frame(1, &win2, &lcd1);
  LCD5110_refresh(&lcd1);

  rect_t r1={3, 8, 50, 33};
  rect_t r1s={10, 10, 2, 2};
  LCD5110_wrect(&r1, 1, &win1, &lcd1);
  LCD5110_wrect(&r1s, 1, &win1, &lcd1);
  LCD5110_wline(5,5, 10, 20, 1, &win2, &lcd1);

  LCD5110_refresh(&lcd1);
  wait_for_button1_pressed();

  LCD5110_wrect_fill(&r1, 1, &win1, &lcd1);
  LCD5110_refresh(&lcd1);
  wait_for_button1_pressed();

  LCD5110_wrect_fill(&r1, 0, &win1, &lcd1);
  LCD5110_refresh(&lcd1);
  wait_for_button1_pressed();

  LCD5110_wprintf(&lcd1, BLACK, &win1, "This is window 1\n");
  LCD5110_refresh(&lcd1);
  wait_for_button1_pressed();

  LCD5110_wprintf(&lcd1, BLACK, &win2, "Window 2 here.\n");
  LCD5110_refresh(&lcd1);
  wait_for_button1_pressed();
  LCD5110_win_draw_frame(1, &win3, &lcd1);
  LCD5110_wprintf(&lcd1, BLACK, &win3, "Owerlapped_windows - window 3 was last.\n");
  LCD5110_refresh(&lcd1);

  lcd2.hw_conf.spi_handle = &hspi3;
  lcd2.hw_conf.spi_cs_pin =  LCD2_CS_Pin;
  lcd2.hw_conf.spi_cs_port = LCD2_CS_GPIO_Port;
  lcd2.hw_conf.rst_pin =  LCD2_RST_Pin;
  lcd2.hw_conf.rst_port = LCD2_RST_GPIO_Port;
  lcd2.hw_conf.dc_pin =  LCD2_DC_Pin;
  lcd2.hw_conf.dc_port = LCD2_DC_GPIO_Port;
  lcd2.def_scr = lcd5110_def_scr;
  LCD5110_init(&lcd2.hw_conf, LCD5110_NORMAL_MODE, 0x40, 2, 3);

  LCD5110_drawBitmap(5, 5, smile, 40, 40, 0, &lcd2.hw_conf);
  LCD5110_refresh(&lcd2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int i = 0;
  while (1)
  {
  /* USER CODE END WHILE */
	  HAL_Delay(70);
	  LCD5110_clear_scr(&lcd2);
	  LCD5110_drawBitmap(5+i, 5, smile, 40, 40, 0, &lcd2.hw_conf);
	  LCD5110_refresh(&lcd2);
	  ++i;
	  if(i>84+40)
		  i=-40;

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void wait_for_button1_pressed()
{
	while( HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET ){}
	HAL_Delay(50);
	while( HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET ){}
	HAL_Delay(50);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
