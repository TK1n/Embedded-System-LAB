/*
 * fsm.c
 *
 *  Created on: Nov 15, 2024
 *      Author: Admin
 */

#include "fsm.h"

uint8_t count_led_flash = 0;
uint8_t flag = 0;
uint8_t timer_hours = 0;
uint8_t timer_min = 0;
uint8_t timer_sec = 0;
uint8_t timer_check = 0;

void displayTime(){
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void displayTimer(){
	lcd_Clear(BLACK);
	lcd_ShowIntNum(70, 100, timer_hours, 2, MAGENTA, BLACK, 24);
	lcd_ShowIntNum(110, 100, timer_min, 2, MAGENTA, BLACK, 24);
	lcd_ShowIntNum(150, 100, timer_sec, 2, MAGENTA, BLACK, 24);
//	lcd_ShowIntNum(20, 100, timer_day, 2, MAGENTA, BLACK, 24);
}

void timer_mode(){
//	timer_hours = ds3231_hours;
//	timer_min = ds3231_min;
//	timer_sec = ds3231_sec;
//	timer_date = ds3231_date;
//	ds3231_Write(ds3231_hours, 0);
//	ds3231_Write(ds3231_min, 0);
//	ds3231_Write(ds3231_sec, 0);
//	ds3231_Write(timer, value)
	count_led_flash = (count_led_flash + 1)%10;
	if(button_count[3] == 40) flag = 1;
	else flag = 0;
	switch(TIMER){
	case timerH:
		displayTimer();
		if(count_led_flash < 5){
			lcd_ShowIntNum(70, 100, timer_hours, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, timer_hours, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_hours = (timer_hours % 24) + 1 ; //btn3 = ↑
			ds3231_Write(ADDRESS_HOUR, timer_hours);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				timer_hours = (timer_hours % 24) + 1;
				ds3231_Write(ADDRESS_HOUR, timer_hours);
			}
		}
		if(button_count[0] == 1) {TIMER = timerM; count_led_flash = 0;}
		if(button_count[14] == 1) status = MODE1;
		break;
	case timerM:
		displayTimer();
		if(count_led_flash < 10){
			lcd_ShowIntNum(70, 100, timer_min, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, timer_min, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_min = (timer_min % 60) + 1; //btn3 = ↑
			ds3231_Write(ADDRESS_MIN, timer_min);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				timer_min = (timer_min % 60) + 1;
				ds3231_Write(ADDRESS_MIN, timer_min);
			}
		}
		if(button_count[0] == 1) {TIMER = timerS; count_led_flash = 0;}
		if(button_count[14] == 1) status = MODE1;
		break;
	case timerS:
		displayTimer();
		if(count_led_flash < 5){
			lcd_ShowIntNum(70, 100, timer_sec, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, timer_sec, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_sec = (timer_sec % 60) + 1; //btn3 = ↑
			ds3231_Write(ADDRESS_SEC, timer_sec);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				timer_sec = (timer_sec % 60) + 1;
				ds3231_Write(ADDRESS_SEC, timer_sec);
			}
		}
		if(button_count[0] == 1) {TIMER = timerH; count_led_flash = 0;}
		if(button_count[14] == 1) status = MODE1;
		break;
	case timerC:
		displayTimer();
		if(count_led_flash < 5){
			lcd_ShowIntNum(70, 100, timer_check, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, timer_check, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_check = (timer_check % 2) + 1;//btn3 = ↑
			ds3231_Write(ADDRESS_DAY, timer_check);
		}
		if(button_count[0] == 1) {TIMER = timerH; count_led_flash = 0;}
		if(button_count[14] == 1) status = MODE1;
		break;
	default:
		break;
	}
}

void alarm(){
	while(timer_check){
		if(ds3231_hours == timer_hours && ds3231_min == timer_min && ds3231_sec == timer_sec){
			lcd_DrawCircle(120, 120, RED, 10, 1);
			timer_check = 0;
		}

	}
	lcd_DrawCircle(120, 120, BLACK, 10, 1);
}

void adjustingTime_mode(){
	count_led_flash = (count_led_flash + 1)%10;
	if(button_count[3] == 40) flag = 1;
	else flag = 0;
	switch(TMP){
	case hour:
		displayTime();
		if(count_led_flash < 5){
			lcd_ShowIntNum(70, 100, ds3231_hours, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_hours = (ds3231_hours % 24) + 1; //btn3 = ↑
			ds3231_Write(ADDRESS_HOUR, ds3231_hours);
		}
		while (flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_hours = (ds3231_hours % 24) + 1; //btn3 = ↑
				ds3231_Write(ADDRESS_HOUR, ds3231_hours);
			}
		}
		if(button_count[0] == 1) {TMP = min; count_led_flash = 0;}
		if(button_count[14] == 1) status = MODE3;
		break;
	case min:
		displayTime();
		if(count_led_flash < 5){
			lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
		} else lcd_ShowIntNum(110, 100, ds3231_min, 2, BLACK, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_min = (ds3231_min % 60) + 1; //btn3 = ↑
			ds3231_Write(ADDRESS_HOUR, ds3231_min);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_min = (ds3231_min % 60) + 1; //btn3 = ↑
				ds3231_Write(ADDRESS_HOUR, ds3231_min);
			}
		}
		if(button_count[0] == 1) TMP = sec;
		if(button_count[14] == 1) status = MODE3;
		break;
	case sec:
		displayTime();
		if(count_led_flash < 5){
			lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
		} else lcd_ShowIntNum(150, 100, ds3231_sec, 2, BLACK, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_sec = (ds3231_sec % 60) + 1; //btn3 = ↑
			ds3231_Write(ADDRESS_HOUR, ds3231_sec);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_sec = (ds3231_sec % 60) + 1; //btn3 = ↑
				ds3231_Write(ADDRESS_HOUR, ds3231_sec);
			}
		}
		if(button_count[0] == 1) TMP = day;
		if(button_count[14] == 1) status = MODE3;
		break;
	case day:
		displayTime();
		if(count_led_flash < 5){
			lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
			lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
		} else {
			lcd_ShowIntNum(20, 130, ds3231_day, 2, BLACK, BLACK, 24);
			lcd_ShowIntNum(70, 130, ds3231_date, 2, BLACK, BLACK, 24);
		}
		if(button_count[3] == 1) {
			ds3231_day = (ds3231_day % 7) + 1;//btn3 = ↑ //display: 6, ds32_day = 4
			ds3231_date = (ds3231_day % 30) + 1;
			ds3231_Write(ADDRESS_DAY, ds3231_day + 2);
			ds3231_Write(ADDRESS_DATE, ds3231_date);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_day = (ds3231_day % 7) + 1;//btn3 = ↑ //display: 6, ds32_day = 4
				ds3231_date = (ds3231_day % 30) + 1;
				ds3231_Write(ADDRESS_DAY, ds3231_day + 2);
				ds3231_Write(ADDRESS_DATE, ds3231_date);
			}
		}
		if(button_count[0] == 1) TMP = month;
		if(button_count[14] == 1) status = MODE3;
		break;
	case month:
		displayTime();
		if(count_led_flash < 5){
			lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
		} else lcd_ShowIntNum(110, 130, ds3231_month, 2, BLACK, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_month = (ds3231_month % 12) +  1; //btn3 = ↑
			ds3231_Write(ADDRESS_MONTH, ds3231_month);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_month = (ds3231_month % 12) +  1; //btn3 = ↑
				ds3231_Write(ADDRESS_MONTH, ds3231_month);
			}
		}
		if(button_count[0] == 1) TMP = year;
		if(button_count[14] == 1) status = MODE3;
		break;
	case year:
		displayTime();
		if(count_led_flash < 10 && flag == 0){
			lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
		} else lcd_ShowIntNum(150, 130, ds3231_year, 2, BLACK, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_year = (ds3231_year % 99) +  1; //btn3 = ↑
			ds3231_Write(ADDRESS_YEAR, ds3231_year);
		}
		while(flag == 1) {
			count_led_flash = 9;
			if(button_count[3]%4 == 0){
				ds3231_year = (ds3231_year % 99) +  1; //btn3 = ↑
				ds3231_Write(ADDRESS_YEAR, ds3231_year);
			}
		}
		if(button_count[0] == 1) TMP = hour;
		if(button_count[14] == 1) status = MODE3;
		break;
	default:
		break;
	}
}


void fsm_run(){
	switch(status){
	case INIT:
		status = MODE1;
	case MODE1:
		ds3231_ReadTime();
		displayTime();
		alarm();
		if(button_count[14]  == 1) status = MODE2; //btn14 = B
		break;

	case MODE2:
		count_led_flash = 0;
		adjustingTime_mode();
		setTimer3(2000);
//		ds3231_ReadTime();
		break;


	case MODE3:
		timer_mode();
		if(button_count[14] == 1) status = MODE1;

	default:
		break;
	}
}
