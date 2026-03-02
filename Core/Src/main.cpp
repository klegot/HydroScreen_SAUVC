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
#include <layouts/bottom_stroke.hpp>
#include <layouts/main_menu.hpp>
#include <layouts/diagnostics_menu.hpp>
#include <layouts/vma_revolutions_menu.hpp>


#include "main.h"

#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_conf.h"

#include "ssd1306_fonts.h"

#include "ssd1306_tests.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t lenght = 128;
uint8_t wight = 64;
uint8_t uart_rx_byte = 0;
char buffer[64] = "";
bool rx_flag = false;
uint8_t buffer_index = 0;


SystemData system_data = {
    .new_vma_statuses = {"---", "---", "---", "---", "---",
                     "---", "---", "---", "---", "---"},
    .light_status = false,
    .batL_voltage = "?",
    .batR_voltage = "?"
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// парсинг для формата: 1110001110,1,12.5,11.8
void ParseData(const char* data)
{
    char temp[64];
    strncpy(temp, data, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    // Разбиваем строку на токены
    char* token = strtok(temp, ",");

    // 1. VMA статусы (10 символов)
    if(token && strlen(token) >= 10)
    {
        for(int i = 0; i < 10; i++)
        {
            if(token[i] == '1')
                strcpy(system_data.new_vma_statuses[i], "OK");
            else if(token[i] == '0')
                strcpy(system_data.new_vma_statuses[i], "ERR");
            else
                strcpy(system_data.new_vma_statuses[i], "---");
        }
    }

    // 2. Статус света
    token = strtok(NULL, ",");
    if(token)
    {
    	if (token[0] == '1') system_data.light_status = true;
    	else system_data.light_status = false;
    }

    // 3. Напряжение BatL
    token = strtok(NULL, ",");
    if(token)
    {
        strncpy(system_data.batL_voltage, token, sizeof(system_data.batL_voltage) - 1);
        //system_data.batL_voltage[sizeof(system_data.batL_voltage) - 1] = '\0';
    }

    // 4. Напряжение BatR
    token = strtok(NULL, ",");
    if(token)
    {
    	int len = strlen(token);
    	for(int i = 0; i < len; i++) // проверка на наличие /n/r
    	{
    	    if(token[i] == '\n' || token[i] == '\r')
    	    {
    	    	token[i] = '\0';
    	    }
    	}
        strncpy(system_data.batR_voltage, token, sizeof(system_data.batR_voltage) - 1);
        //system_data.batR_voltage[sizeof(system_data.batR_voltage) - 1] = '\0';
    }
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
  BottomSTR bottom_str;
  bottom_str.Draw();



  BaseMenu* current_window = nullptr;
  current_window = new MainMenu();
  current_window->Draw();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  bottom_str.Draw();
	  if(rx_flag)
	      {
		  	  rx_flag = false;

		      // ЭХО
		      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		      HAL_Delay(2);
		      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, buffer_index, 100);
		      uint8_t newline[] = "\r\n";
		      HAL_UART_Transmit(&huart1, newline, 2, 100);
		      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

		      buffer[0] = '\0';
		      buffer_index = 0;

		      HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);

		      current_window->DataUpdate(&system_data);
		      current_window->Draw();

		      //bottom_str.DataUpdate(&system_data);
		      //bottom_str.Draw();
	      }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) // вниз
	  {
		  current_window->CursorDown();
		  HAL_Delay(100);
	  }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) //выбор
	  {
		  if(current_window->GetType() == BaseMenu::MAIN_MENU)
		  {
			  if (current_window->Enter() == 1)
			  {

			  }
			  if (current_window->Enter() == 2)
			  {
			  	delete current_window;
			  	current_window = new DiagnosticsMenu();
			  	current_window->DataUpdate(&system_data);
			  	current_window->Draw();
			  }
			  if (current_window->Enter() == 3)
			  {

			  }
		  }
		  else if(current_window->GetType() == BaseMenu::DIAGNOSTICS_MENU)
		  {
			  delete current_window;
			  current_window = new VmaMenu();
			  current_window->DataUpdate(&system_data);
			  current_window->Draw();
		  }

		  HAL_Delay(100);

	  }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // вверх
	  {
		  current_window->CursorUp();
		  HAL_Delay(100);

	  }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) // отмена
	  {
		  delete current_window;
		  current_window = new MainMenu();
		  current_window->Draw();
		  HAL_Delay(100);

	  }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)) // средняя нижняя
	  {
		  HAL_Delay(100);
	  }
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) // правая нижняя
	  {
		  HAL_Delay(100);
	  }
	  ssd1306_UpdateScreen();

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
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        if(buffer_index < sizeof(buffer) - 1)
        {
            buffer[buffer_index++] = (char)uart_rx_byte;
            buffer[buffer_index] = '\0';

            if(uart_rx_byte == '\n' || uart_rx_byte == '\r')
            {
                rx_flag = true;
                ParseData(buffer);
            }
        }

        HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
    }
}
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
