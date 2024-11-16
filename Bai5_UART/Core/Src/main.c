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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "uart.h"

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
void system_init();
void test_LedDebug();
void test_Uart();
void displayTime();
void updateTime();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void test_button();  // Function prototype for test_button
void display_lcd();
void adjustTime();
void setTimer();
void checkAlarm();
void update_time();
void clock_mode();

typedef enum {
    TIME_DISPLAY,
    TIME_ADJUST,
    TIMER_MODE,
	TIME_UPDATE
} ClockMode;

ClockMode currentMode = TIME_DISPLAY;
uint32_t lastButtonPressTime = 0;

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcd_Clear(BLACK);
  updateTime();
  while (1)
  {
    /* USER CODE END WHILE */
	  while(!flag_timer2);
	  flag_timer2 = 0;
	  button_Scan();
//	   test_LedDebug();
	  ds3231_ReadTime();
//	   test_Uart();
    clock_mode();
	  // ds3231_Update();
//	  printf(ds3231_buffer[0]); //second
//	  printf(ds3231_buffer[1]); //second
//	  printf(ds3231_buffer[2]); //second
//	  printf(ds3231_buffer[3]); //second
//	  printf(ds3231_buffer[4]); //second
//	  printf(ds3231_buffer[5]); //second
//	  printf(ds3231_buffer[6]); //second

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
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  timer_init();
	  led7_init();
	  button_init();
	  lcd_init();
	  ds3231_init();
	  uart_init_rs232();
	  setTimer2(50);
}

uint16_t count_led_debug = 0;

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

void test_button(){
	for(int i = 0; i < 16; i++){
		if(button_count[i] == 1){
			led7_SetDigit(i/10, 2, 0);
			led7_SetDigit(i%10, 3, 0);
      lcd_ShowIntNum(140, 105, i, 2, BRED, WHITE, 32);
		}
	}
}

void test_lcd() {
    lcd_Fill(0, 0, 240, 20, BLUE);  // Fills a rectangle at the top of the screen
    lcd_StrCenter(0, 2, (uint8_t *)"Hello World !!!", RED, BLUE, 16, 1);  // Displays centered text
    lcd_ShowStr(20, 30, (uint8_t *)"Test lcd screen", WHITE, RED, 24, 0);  // Displays a string
    lcd_DrawCircle(60, 120, GREEN, 40, 1);  // Draws a filled green circle
    lcd_DrawCircle(160, 120, BRED, 40, 0);  // Draws an unfilled red circle
    lcd_ShowPicture(80, 200, 90, 90, gImage_l_flag);  // Displays an image at given coordinates
}

void test_Uart(){
	if(button_count[12] == 1){
		uart_Rs232SendNum(ds3231_hours);
		uart_Rs232SendString(":");
		uart_Rs232SendNum(ds3231_min);
		uart_Rs232SendString(":");
		uart_Rs232SendNum(ds3231_sec);
		uart_Rs232SendString("\n");
	}
}

uint8_t isButtonUp() {
    return (button_count[3] % 10 == 1) ? 1 : 0;
}

uint8_t flag;
uint8_t isButtonUp2s() {
	if(button_count[3] % 10 == 1 && button_count[3]/10 > 1){
		flag = 1;
	}else {
		flag = 0;
	}
	return flag;
}

uint8_t isButtonDown() {
    return (button_count[7] == 1) ? 1 : 0;
}

uint8_t timerMode = 0;
uint8_t adjustMode = 0; // 0 = hours, 1 = minutes, 2 = seconds
uint8_t counter = 0;
uint16_t cnt;
uint8_t idx = 0;
//uint8_t update_arr[7];
//char time_features[7][10] = {"Seconds", "Minutes", "Hours", "Days", "Dates", "Months", "Years"};
//char message[30];

void display_lcd() {
    switch (currentMode) {
        case TIME_DISPLAY:
            lcd_Fill(0, 0, 240, 20, BLUE);
            lcd_StrCenter(0, 2, (uint8_t *)"This is now", RED, BLUE, 16, 1);
            displayTime();  // Displays the current time
            timerMode = 0;
            adjustMode = 0;
            checkAlarm();
            break;

        case TIME_ADJUST:
            lcd_Fill(0, 0, 240, 20, BLUE);
            lcd_StrCenter(0, 2, (uint8_t *)"Please edit the time", RED, BLUE, 16, 1);
            adjustTime();  // Allows time adjustment
            break;

        case TIMER_MODE:
            lcd_Fill(0, 0, 240, 20, BLUE);
            lcd_StrCenter(0, 2, (uint8_t *)"Please set timer", RED, BLUE, 16, 1);
            setTimer();  // Sets the timer
            break;
        case TIME_UPDATE:
        	lcd_Fill(0, 0, 240, 20, BLUE);
			lcd_StrCenter(0, 2, (uint8_t *)"Please update the time", RED, BLUE, 16, 1);
			update_time();  // Time_update
//			uart_Rs232SendNum(cnt);
			break;
    }
}

void updateTime(){
	ds3231_Write(ADDRESS_YEAR, 23);
	ds3231_Write(ADDRESS_MONTH, 10);
	ds3231_Write(ADDRESS_DATE, 20);
	ds3231_Write(ADDRESS_DAY, 6);
	ds3231_Write(ADDRESS_HOUR, 20);
	ds3231_Write(ADDRESS_MIN, 11);
	ds3231_Write(ADDRESS_SEC, 23);
}

uint8_t hours;
uint8_t min;
uint8_t sec;
uint8_t day;
uint8_t date;
uint8_t month;
uint8_t year;

void displaySec(int num, int isBlink) {
	if (isBlink) {
	    counter = (counter + 1)%10;
	    if(counter < 5) {
//	    	lcd_ShowStr(150, 100, "  ", 2, BLACK, BLACK, 24);
	    	lcd_ShowIntNum(150, 100, num, 2, BLACK, BLACK, 24);
	    }
	    else {
	    	lcd_ShowIntNum(150, 100, num, 2, GREEN, BLACK, 24);
	    }
	}
	else {
		lcd_ShowIntNum(150, 100, num, 2, GREEN, BLACK, 24);
	}
}

void displayMin(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
//			lcd_ShowStr(110, 100, "  ", 2, GREEN, BLACK, 24);
			lcd_ShowIntNum(110, 100, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(110, 100, num, 2, GREEN, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(110, 100, num, 2, GREEN, BLACK, 24);
	}
}

void displayHour(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
			lcd_ShowIntNum(70, 100, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(70, 100, num, 2, GREEN, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(70, 100, num, 2, GREEN, BLACK, 24);
	}
}


void displayDay(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
			lcd_ShowIntNum(20, 130, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(20, 130, num, 2, YELLOW, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(20, 130, num, 2, YELLOW, BLACK, 24);
	}
}

void displayDate(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
			lcd_ShowIntNum(70, 130, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(70, 130, num, 2, YELLOW, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(70, 130, num, 2, YELLOW, BLACK, 24);
	}
}

void displayMonth(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
			lcd_ShowIntNum(110, 130, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(110, 130, num, 2, YELLOW, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(110, 130, num, 2, YELLOW, BLACK, 24);
	}
}

void displayYear(int num, int isBlink) {
	if (isBlink) {
		counter = (counter + 1)%10;
		if(counter < 5) {
			lcd_ShowIntNum(150, 130, num, 2, BLACK, BLACK, 24);
		}
		else {
			lcd_ShowIntNum(150, 130, num, 2, YELLOW, BLACK, 24);
		}
	}
	else {
		lcd_ShowIntNum(150, 130, num, 2, YELLOW, BLACK, 24);
	}
}


//void blinkTime(uint8_t hours, uint8_t min, uint8_t sec, uint8_t day, uint8_t date, uint8_t month, uint8_t year){
//
//	lcd_ShowIntNum(70, 100, hours, 2, GREEN, BLACK, 24);
//	lcd_ShowIntNum(110, 100, min, 2, GREEN, BLACK, 24);
//	lcd_ShowIntNum(150, 100, sec, 2, GREEN, BLACK, 24);
//	lcd_ShowIntNum(20, 130, day, 2, YELLOW, BLACK, 24);
//	lcd_ShowIntNum(70, 130, date, 2, YELLOW, BLACK, 24);
//	lcd_ShowIntNum(110, 130, month, 2, YELLOW, BLACK, 24);
//	lcd_ShowIntNum(150, 130, year, 2, YELLOW, BLACK, 24);
//}

void editTime(uint8_t hours, uint8_t min, uint8_t sec, uint8_t day, uint8_t date, uint8_t month, uint8_t year){
	lcd_ShowIntNum(70, 100, hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, year, 2, YELLOW, BLACK, 24);
}


void displayTime(){
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void adjustTime() {

    // Check for button presses
//	uart_Rs232SendNum(counter);
//	uart_Rs232SendNum(flag_timer2);
	if (flag_timer2) {
		counter++;
	}
    if (isButtonUp() && flag == 0) {

        // Increment current field
    	if (adjustMode == 0) {
//			if (counter == 5) {
//				lcd_ShowStr(150, 100, "  ", 2, GREEN, BLACK, 24);
//			}
//			else if (counter == 10) {
//				lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
//				counter = 0;
//			}
			sec = (sec + 1) % 60;
        } else if (adjustMode == 1) {
//        	if (counter == 5) {
//        		lcd_ShowStr(110, 100, "  ", 2, GREEN, BLACK, 24);
//			}
//        	else if (counter == 10) {
//				lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
//				counter = 0;
//			}
            min = (min + 1) % 60;
        } else if (adjustMode == 2) {
//        	if (counter == 5) {
//        		lcd_ShowStr(70, 100, "  ", 2, GREEN, BLACK, 24);
//			}
//        	else if (counter == 10) {
//				lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
//				counter = 0;
//			}
            hours = (hours + 1) % 24;
        } else if (adjustMode == 3) {
//        	if (counter == 5) {
//        		lcd_ShowStr(20, 130, "  ", 2, YELLOW, BLACK, 24);
//				lcd_ShowStr(70, 130, "  ", 2, YELLOW, BLACK, 24);
//			}
//        	else if (counter == 10) {
//				lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
//				lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
//				counter = 0;
//			}
//        	date = (date + 1) % 30;
//        	if (!date) {
//        		date = 30;
//        	}
//        	day = (day + 1) % 7;
//        	if (day = 0) {
//        		day = 7;
//        	} else if (day = 1) {
//        		day = 8;
//        	}
        	date = (date + 1) % 30;
			if (!date) {
				date = 30;}
			day = (day + 1) % 7;
			if (day == 0) {
				day = 7;}
			else if (day == 1) {
				day = 8;}
        } else if (adjustMode == 4) {
//        	if (counter == 5) {
//        		lcd_ShowStr(110, 130, "  ", 2, YELLOW, BLACK, 24);
//			}
//        	else if (counter == 10) {
//				lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
//				counter = 0;
//			}
        	month = (month + 1) % 12;
        	if (!month) {
        		month = 12;
        	}
        }else if (adjustMode == 5) {
//        	if (counter == 5) {
//        		lcd_ShowStr(150, 130, "  ", 2, YELLOW, BLACK, 24);
//			}
//        	else if (counter == 10) {
//				lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
//				counter = 0;
//			}
        	year = (year + 1) % 100;
        }
//    	if (button_count[12] == 1) {  // Button to cycle fields
//			adjustMode = (adjustMode + 1) % 6;  // Cycle through hours, minutes, seconds
//			counter = 0;
//			ds3231_Write(ADDRESS_YEAR, year);
//			ds3231_Write(ADDRESS_MONTH, month);
//			ds3231_Write(ADDRESS_DATE, date);
//			ds3231_Write(ADDRESS_DAY, day);
//			ds3231_Write(ADDRESS_HOUR, hours);
//			ds3231_Write(ADDRESS_MIN, min);
//			ds3231_Write(ADDRESS_SEC, sec);
//		}

//		editTime(hours, min, sec, day, date, month, year);

    }
//    if (isButtonUp2s()){
//    	if (adjustMode == 0) {
//			sec = (sec + 1) % 60;
//
//		} else if (adjustMode == 1) {
//			min = (min + 1) % 60;
//		} else if (adjustMode == 2) {
//			hours = (hours + 1) % 24;
//		} else if (adjustMode == 3) {
//			day = (day + 1) % 7 + 2;
//		} else if (adjustMode == 4) {
//			date = (date + 1) % 30;
//		}else if (adjustMode == 5) {
//			month = (month + 1) % 12;
//		} else if (adjustMode == 6) {
//			year = (year + 1) % 99;
//		}
//
//
//
//	}

    if (button_count[12] == 1) {  // Button to cycle fields
		adjustMode = (adjustMode + 1) % 6;  // Cycle through hours, minutes, seconds
		counter = 0;
		ds3231_Write(ADDRESS_YEAR, year);
		ds3231_Write(ADDRESS_MONTH, month);
		ds3231_Write(ADDRESS_DATE, date);
		ds3231_Write(ADDRESS_DAY, day);
		ds3231_Write(ADDRESS_HOUR, hours);
		ds3231_Write(ADDRESS_MIN, min);
		ds3231_Write(ADDRESS_SEC, sec);

	}
//    editTime(hours, min, sec, day, date, month, year);
	displaySec(sec, adjustMode == 0);
	displayMin(min, adjustMode == 1);
	displayHour(hours, adjustMode == 2);
	displayDay(day, adjustMode == 3);
	displayDate(date, adjustMode == 3);
	displayMonth(month, adjustMode == 4);
	displayYear(year, adjustMode == 5);
    // Move to the next field when another button is pressed

}


uint8_t timer_hours = 0;
uint8_t timer_minutes = 0;
uint8_t timer_seconds = 0;
uint8_t timer_day = 0;
uint8_t timer_date = 0;
uint8_t timer_month = 0;
uint8_t timer_year = 0;
  // 0 = hours, 1 = minutes, 2 = seconds

void setTimer() {
    // Increment or decrement the current field
	if (flag_timer2){
		counter++;
	}
    if (isButtonUp()) {
    	if (timerMode == 0) {
//    		lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
    		timer_seconds = (timer_seconds + 1) % 60;
		} else if (timerMode == 1) {
			timer_minutes = (timer_minutes + 1) % 60;
		} else if (timerMode == 2) {
			timer_hours = (timer_hours + 1) % 24;
		} else if (timerMode == 3) {
			timer_date = (timer_date + 1) % 30;
			if (!timer_date) {
				timer_date = 30;}
			timer_day = (timer_day + 1) % 7;
			if (timer_day == 0) {
				timer_day = 7;}
			else if (timer_day == 1) {
				timer_day = 8;}
		} else if (timerMode == 4) {
			timer_month = (timer_month + 1) % 12;
			if (!timer_month) {
				timer_month = 12;}
		}else if (timerMode == 5) {
			timer_year = (timer_year + 1) % 100;
    }
    }


    if (button_count[12] == 1) {  // Button to cycle fields
    	timerMode = (timerMode + 1) % 6;  // Cycle through hours, minutes, seconds
    	counter = 0;
//		ds3231_Write(ADDRESS_HOUR, timer_hours);
//		ds3231_Write(ADDRESS_MIN, timer_minutes);
//		ds3231_Write(ADDRESS_SEC, timer_seconds);
//		ds3231_Write(ADDRESS_YEAR, timer_year);
//		ds3231_Write(ADDRESS_MONTH, timer_month);
//		ds3231_Write(ADDRESS_DATE, timer_date);
//		ds3231_Write(ADDRESS_DAY, timer_day);
	}

//    editTime(timer_hours, timer_minutes, timer_seconds, timer_day, timer_date, timer_month, timer_year);
    displaySec(timer_seconds, timerMode == 0);
	displayMin(timer_minutes, timerMode == 1);
	displayHour(timer_hours, timerMode == 2);
	displayDay(timer_day, timerMode == 3);
	displayDate(timer_date, timerMode == 3);
	displayMonth(timer_month, timerMode == 4);
	displayYear(timer_year, timerMode == 5);
}

void clock_mode(uint32_t currentMillis) {
    // Check if button is pressed and if debounce period has passed
    if (button_count[0] == 1 ) {
        // Cycle through modes
//    	currentMode++;
    	currentMode = (currentMode + 1) % 4;
//    	if (currentMode == TIME_UPDATE)
//    		currentMode = TIME_DISPLAY;
    	if (currentMode == TIME_ADJUST){
        counter = 0;
    		ds3231_ReadTime();
    		hours = ds3231_hours;
			min = ds3231_min;
			sec = ds3231_sec;
			day = ds3231_day;
			date = ds3231_date;
			month = ds3231_month;
			year = ds3231_year;
			timer_hours = ds3231_hours;
			timer_minutes = ds3231_min;
			timer_seconds = ds3231_sec;
			timer_day = ds3231_day;
			timer_date = ds3231_date;
			timer_month = ds3231_month;
			timer_year = ds3231_year;

    	}
    	else if (currentMode == TIMER_MODE){
    		counter = 0;
    	}
      else if (currentMode == TIME_UPDATE){
//    	printf("%d", cnt);
    	UART_flag = 0;
    	cnt_uart = 0;
        cnt = 3 * 200;
        idx = 0;
//    	uart_Rs232SendNum(cnt);
      }
//			else if (currentMode == TIMER_MODE){
//    		ds3231_ReadTime();
//    		timer_hours = ds3231_hours;
//    		timer_minutes = ds3231_min;
//    		timer_seconds = ds3231_sec;
//    	}
    }
    display_lcd();

}

// Alarm checking function for Timer Mode
void checkAlarm() {
    if(ds3231_year == timer_year &&
       ds3231_month == timer_month &&
	   ds3231_date == timer_date &&
	   ds3231_day == timer_day &&
       ds3231_hours == timer_hours &&
	   ds3231_min == timer_minutes &&
	   ds3231_sec >= timer_seconds &&
	   ds3231_sec < timer_seconds+5){
    	lcd_Fill(0, 0, 240, 20, BLUE);
		lcd_StrCenter(0, 2, (uint8_t *)"Alarm !!!!!!!!!!! ", RED, BLUE, 16, 1);
    }
//    timer_hours = ds3231_hours;
//	timer_minutes = ds3231_min;
//	timer_seconds = ds3231_sec;
}

//uint8_t str2dec(uint8_t)

void update_time() {
uint8_t update_arr[7];
uint8_t val;
char time_features[7][10] = {"Seconds", "Minutes", "Hours", "Days", "Dates", "Months", "Years"};
char message[30];
	while (1) {
        if (flag_timer2) {
             flag_timer2 = 0;
            if (cnt % 200 == 0) {
                sprintf(message, "Updating %s ...", time_features[idx]);
                lcd_ShowStr(20, 30, (uint8_t *)message, WHITE, RED, 24, 0);

                sprintf(message, "\r\n%s: ", time_features[idx]);
                uart_Rs232SendString((uint8_t *)message);
            }
            cnt--;
            if (cnt == 0) {
            	strcpy(message, "                   ");
				lcd_ShowStr(20, 30, message, WHITE, BLACK, 24, 0);
                currentMode = TIME_DISPLAY;
                break;
            }

        }

            if (UART_flag) {
            	UART_flag = 0;
                if ((strcmp(time_features[idx], "Seconds") == 0 && (rx_msg < 0 || rx_msg > 60)) ||
                    (strcmp(time_features[idx], "Minutes") == 0 && (rx_msg < 0 || rx_msg > 60)) ||
                    (strcmp(time_features[idx], "Hours") == 0 && (rx_msg < 0 || rx_msg > 24)) ||
                    (strcmp(time_features[idx], "Days") == 0 && (rx_msg < 1 || rx_msg > 8)) ||
                    (strcmp(time_features[idx], "Dates") == 0 && (rx_msg < 1 || rx_msg > 30)) ||
                    (strcmp(time_features[idx], "Months") == 0 && (rx_msg < 1 || rx_msg > 12)) ||
                    (strcmp(time_features[idx], "Years") == 0 && (rx_msg < 0 || rx_msg > 99))) {

                        strcpy(message, "Invalid value\r\n");
                        lcd_StrCenter(0, 2, (uint8_t *)message, RED, BLUE, 16, 1);
                        uart_Rs232SendString(message);
                        cnt = 3 * 200;
                }
                else {
                  update_arr[idx] = rx_msg;
                  cnt = 3 * 200;
                  idx++;
                }
            }

            if (idx == 7) {
            	strcpy(message, "                   ");
              lcd_ShowStr(20, 30, message, WHITE, BLACK, 24, 0);
              ds3231_Write(ADDRESS_YEAR, update_arr[6]);
              ds3231_Write(ADDRESS_MONTH, update_arr[5]);
              ds3231_Write(ADDRESS_DATE, update_arr[4]);
              ds3231_Write(ADDRESS_DAY, update_arr[3]);
              ds3231_Write(ADDRESS_HOUR, update_arr[2]);
              ds3231_Write(ADDRESS_MIN, update_arr[1]);
              ds3231_Write(ADDRESS_SEC, update_arr[0]);
              ds3231_ReadTime();
				currentMode = TIME_DISPLAY;
				break;
            }
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
