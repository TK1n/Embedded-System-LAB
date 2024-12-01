/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "uart.h"
#include "gpio.h"
#include "fsmc.h"
#include <stdio.h>
#include <math.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "sensor.h"
#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAPTURE_SIZE 3
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
void system_init();
void plot_init();
void updateTime();
void test_LedDebug();
void Buzzer();
void Adc_Connect();
void displayTime();
void send_Uart();
void lcd_Display();
void plot_update();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_TIM13_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  system_init();
//  updateTime();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  lcd_Clear(BLACK);
  plot_init();
  while (1)
  {
	  while(!flag_timer2);
	  flag_timer2 = 0;
	  button_Scan();
//	  test_LedDebug();
	  displayTime();
//	  ds3231_ReadTime();
	  Adc_Connect();
//	  test_Uart();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init(){
	  timer_init();
	  button_init();
	  lcd_init();
	  sensor_init();
	  buzzer_init();
	  uart_init_rs232();
	  ds3231_init();
	  updateTime();
	  ds3231_ReadTime();
	  displayTime();
	  setTimer2(50);
}

uint8_t count_led_debug = 0;

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

uint8_t isButtonUp()
{
    if (button_count[3] == 1)
        return 1;
    else
        return 0;
}

uint8_t isButtonDown()
{
    if (button_count[7] == 1)
        return 1;
    else
        return 0;
}

uint8_t isButtonRight()
{
    if (button_count[11] == 1)
        return 1;
    else
        return 0;
}

void updateTime(){
	ds3231_Write(ADDRESS_MIN, 0);
	ds3231_Write(ADDRESS_SEC, 0);
}

void displayTime(){
	led7_SetDigit(ds3231_min/10, 0, 0);
	led7_SetDigit(ds3231_min%10, 1, 0);
	led7_SetDigit(ds3231_sec/10, 2, 0);
	led7_SetDigit(ds3231_sec%10, 3, 0);
}

float power = 0;
int light = 0;
float temp = 0;
float humi = 0;
char degreeSymbol[] = "\u00B0";

int power_array[CAPTURE_SIZE] = {0};
int capture_index = 0;
uint8_t count_adc = 0;
uint8_t power_capture_count = 0;
uint8_t flag = 0;

char message[30];

void plot_init(){
	lcd_DrawLine(30,  150, 30, 250, GREEN);  // power
	lcd_ShowChar(10,  150, 'P', GREEN, BLACK, 16, 0);
	lcd_ShowChar(10,  170, 'O', GREEN, BLACK, 16, 0);
	lcd_ShowChar(10,  190, 'W', GREEN, BLACK, 16, 0);
	lcd_ShowChar(10,  210, 'E', GREEN, BLACK, 16, 0);
	lcd_ShowChar(10,  230, 'R', GREEN, BLACK, 16, 0);
	lcd_ShowStr(10,   130, "300 mW", GREEN, BLACK, 16, 0);
	lcd_DrawLine(30,  250, 210, 250, GREEN); // time
	lcd_ShowChar(90,  280, 'T', GREEN, BLACK, 16, 0);
	lcd_ShowChar(110, 280, 'I', GREEN, BLACK, 16, 0);
	lcd_ShowChar(130, 280, 'M', GREEN, BLACK, 16, 0);
	lcd_ShowChar(150, 280, 'E', GREEN, BLACK, 16, 0);
	lcd_ShowStr(210,  260, "15s", GREEN, BLACK, 16, 0);
	lcd_ShowStr(50,   260, "2", GREEN, BLACK, 16, 0);
	lcd_ShowStr(100,  260, "7", GREEN, BLACK, 16, 0);
	lcd_ShowStr(150,  260, "12", GREEN, BLACK, 16, 0);
}

void Adc_Connect(){
	count_adc = (count_adc + 1)%20; // 1s;
	if(count_adc == 0){
		sensor_Read();
		power = (sensor_GetVoltage() * sensor_GetCurrent());
		light = sensor_GetLight();
		temp = sensor_GetTemperature();
		humi = (sensor_GetPotentiometer()/40.96);
		ds3231_ReadTime();
		lcd_Display();
		send_Uart();
		if (power_capture_count == 2 || power_capture_count == 7 || power_capture_count == 12) {
			power_array[capture_index] = round(power) ;
			capture_index = (capture_index + 1) % CAPTURE_SIZE;
		}
		if (power_capture_count == 15) {
			plot_update();
			power_capture_count = 0;
		}
		power_capture_count = (power_capture_count + 1);
	}
	if(light >= 200) lcd_ShowStr(130, 50, "Strong", RED, BLACK, 16, 0);
	else lcd_ShowStr(130, 50, "Weak", RED, BLACK, 16, 0);
	if(humi >= 70) flag = 1;
	else flag = 0;
	if(flag == 1 && count_adc <= 9){
		buzzer_SetVolume(25);
	}else buzzer_SetVolume(0);
}

uint8_t buzzer_count = 0;
void Buzzer(){
//	buzzer_count = (buzzer_count + 1) % 20;
//	if(buzzer_count < 10) buzzer_SetVolume(50);
//	else buzzer_SetVolume(0);
	if(isButtonUp()){
		buzzer_SetVolume(50);
	}

	if(isButtonDown()){
		buzzer_SetVolume(0);
	}

	if(isButtonRight()){
		buzzer_SetVolume(25);
	}

}


void send_Uart(){
	if(humi >= 70){
		sprintf(message,"\r\nHumidity > 70%%");
		uart_Rs232SendString((uint8_t *)message);
	} else {
	sprintf(message,"\r\nPower: \t%4.2f\t(mW)", power);
 	uart_Rs232SendString((uint8_t *)message);
	if(light >= 200) sprintf(message,"\r\nLight: \tStrong");
	else sprintf(message,"\r\nLight: \tWeak");
	uart_Rs232SendString((uint8_t *)message);
	sprintf(message,"\r\nTemperature: \t%4.2f\t(%cC)", temp, 186);
	uart_Rs232SendString((uint8_t *)message);
	sprintf(message,"\r\nHumidity: \t%4.2f\t(%%)", humi);
	uart_Rs232SendString((uint8_t *)message);
	sprintf(message, "\r\n---------------------------");
	uart_Rs232SendString((uint8_t *)message);
	}
}

void lcd_Display(){
	lcd_ShowStr(10, 30, "Power:", RED, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 30, power , 5, RED, BLACK, 16);
	lcd_ShowStr(200, 30, "mW", RED, BLACK, 16, 0);
	lcd_ShowStr(10, 50, "Light:", RED, BLACK, 16, 0);
//	lcd_ShowIntNum(130, 50, light, 3, RED, BLACK, 16);
	lcd_ShowStr(10, 70, "Temperature:", RED, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 70, temp, 4, RED, BLACK, 16);
	lcd_DrawCircle(196, 74, RED, 2, 0);
	lcd_ShowStr(200, 70, "C", RED, BLACK, 16, 0);
	lcd_ShowStr(10, 90, "Humidity:", RED, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 90, humi, 4, RED, BLACK, 16);
	lcd_ShowStr(200, 90, "%", RED, BLACK, 16, 0);
//	lcd_DrawLine(40, 290 - 100 * 0.3, 70, 290 - 100*0.6 , BLUE);
}

void plot_update(){
	lcd_Fill(30, 150, 240, 250, BLACK);// clear plot
	lcd_DrawLine(30, 150, 30, 250, GREEN);  // power
	lcd_DrawLine(30, 250, 210, 250, GREEN); // time

	lcd_DrawLine(50,   (250 - power_array[0]/3 ) , 100,  (250 - power_array[1]/3 ) ,  BLUE);
	lcd_DrawLine(100,  (250 - power_array[1]/3 ) , 150,  (250 - power_array[2]/3 ) ,  BLUE);
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
