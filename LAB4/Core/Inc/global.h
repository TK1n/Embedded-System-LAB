/*
 * global.h
 *
 *  Created on: Nov 15, 2024
 *      Author: Admin
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "button.h"
#include "ds3231.h"
#include "fsmc.h"
#include "gpio.h"
#include "i2c.h"
#include "lcd.h"
#include "led_7seg.h"
#include "main.h"
#include "picture.h"
#include "software_timer.h"
#include "spi.h"
#include "tim.h"
#include "utils.h"

#define INIT 	1
#define MODE1 	2
#define MODE2	3
#define MODE3	4

extern int status;

#define hour	1
#define min 	2
#define sec 	3
#define day		4
#define month	5
#define year	6

extern int TMP;

#define timerH 1
#define timerM 2
#define timerS 3
#define timerC 4

extern int TIMER;


#define ADDRESS_SEC			0x00
#define ADDRESS_MIN			0x01
#define ADDRESS_HOUR		0x02
#define ADDRESS_DAY			0x03
#define ADDRESS_DATE		0x04
#define ADDRESS_MONTH		0x05
#define ADDRESS_YEAR		0x06

extern uint8_t ds3231_hours;
extern uint8_t ds3231_min;
extern uint8_t ds3231_sec;
extern uint8_t ds3231_date;
extern uint8_t ds3231_day;
extern uint8_t ds3231_month;
extern uint8_t ds3231_year;

extern uint8_t count_led_flash;
extern uint8_t flag;

extern uint8_t timer_hours;
extern uint8_t timer_min;
extern uint8_t timer_sec;
extern uint8_t timer_check;


#endif /* INC_GLOBAL_H_ */
