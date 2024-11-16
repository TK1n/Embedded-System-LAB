/*
 * fsm.c
 *
 *  Created on: Nov 15, 2024
 *      Author: Admin
 */

#include "fsm.h"
#include "global.h"

/*
 * button_count[0] is button with label "1" on the kit
 * button_count[3] is button with label "↑" on the kit
 * button_count[14] is button with label "B" on the kit
 */


uint8_t count_increase = 0;
uint8_t count_led_flash = 0;
uint8_t flag = 0;
uint8_t timer_hours = 0;
uint8_t timer_min = 0;
uint8_t timer_sec = 0;
uint8_t timer_check = 0;

void updateTime(){
	ds3231_Write(ADDRESS_YEAR, 23);
	ds3231_Write(ADDRESS_MONTH, 10);
	ds3231_Write(ADDRESS_DATE, 20);
	ds3231_Write(ADDRESS_DAY, 6);
	ds3231_Write(ADDRESS_HOUR, 12);
	ds3231_Write(ADDRESS_MIN, 59);
	ds3231_Write(ADDRESS_SEC, 50);
}

void testAlarm(){
	timer_hours = 13;
	timer_min = 0;
	timer_sec = 0;
	timer_check = 1;
}

void displayTime(){
	lcd_StrCenter(0, 2, "NORMAL TIME", RED, BLACK, 16, 1);
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void displayAdjTime(){
	lcd_StrCenter(0, 2, "ADJUSTING TIME MODE", RED, BLACK, 16, 1);
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void displayTimer(){
	lcd_ShowIntNum(70, 100, timer_hours, 2, MAGENTA, BLACK, 24);
	lcd_ShowIntNum(110, 100, timer_min, 2, MAGENTA, BLACK, 24);
	lcd_ShowIntNum(150, 100, timer_sec, 2, MAGENTA, BLACK, 24);
	lcd_StrCenter(0, 2, "TIMER MODE", RED, BLACK, 16, 1);
	lcd_ShowIntNum(20, 100, timer_check, 2, MAGENTA, BLACK, 24);
}

void clockTime(){
	ds3231_Write(ADDRESS_HOUR, ds3231_hours);
	ds3231_Write(ADDRESS_MIN, ds3231_min);
	ds3231_Write(ADDRESS_SEC, ds3231_sec);
	ds3231_Write(ADDRESS_DAY, ds3231_day);
}

void timer_mode(){
	switch(TIMER){
	case init:
		displayTimer();
		TIMER = timerH;
		break;
	case timerH:
		if(count_led_flash < 10){
			lcd_ShowIntNum(70, 100, timer_hours, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, timer_hours, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_hours = (timer_hours + 1) %24;
			ds3231_Write(ADDRESS_HOUR, timer_hours);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			timer_hours = (timer_hours + 1) %24;
			ds3231_Write(ADDRESS_HOUR, timer_hours);
		}
		if(button_count[0] == 1) {TIMER = timerM; count_led_flash = 0; displayTimer();}
		break;
	case timerM:
		if(count_led_flash < 10){
			lcd_ShowIntNum(110, 100, timer_min, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(110, 100, timer_min, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_min = (timer_min + 1) % 60;
			ds3231_Write(ADDRESS_MIN, timer_min);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			timer_min = (timer_min + 1) % 60;
			ds3231_Write(ADDRESS_MIN, timer_min);
		}
		if(button_count[0] == 1) {TIMER = timerS; count_led_flash = 0; displayTimer();}
		break;
	case timerS:
		if(count_led_flash < 10){
			lcd_ShowIntNum(150, 100, timer_sec, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(150, 100, timer_sec, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_sec = (timer_sec + 1) % 60;
			ds3231_Write(ADDRESS_SEC, timer_sec);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;

			timer_sec = (timer_sec + 1) % 60;
			ds3231_Write(ADDRESS_SEC, timer_sec);
		}
		if(button_count[0] == 1) {TIMER = timerC; count_led_flash = 0; displayTimer();}
		break;
	case timerC:
		if(count_led_flash < 10){
			lcd_ShowIntNum(20, 100, timer_check, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(20, 100, timer_check, 2, MAGENTA, BLACK, 24);
		if(button_count[3] == 1) {
			timer_check = (timer_check + 1) %2;
			ds3231_Write(ADDRESS_DAY, timer_check);
		}
		if(button_count[0] == 1) {TIMER = timerH; count_led_flash = 0; displayTimer();}
		break;
	default:
		break;
	}
}

void alarm(){
	if(timer_check == 1 && ds3231_hours == timer_hours && ds3231_min == timer_min && ds3231_sec == timer_sec){
		lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
		lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
		lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
		lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
		lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
		lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
		lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
		DrawTestPage("ALARM");
		if(button_count[1] == 1) {timer_check = 0; lcd_Clear(BLACK); status = MODE1;}
	} else {
		ds3231_ReadTime();
		displayTime();
	}
}

void adjustingTime_mode(){
	switch(TMP){
	case hour:
		if(count_led_flash < 10){
			lcd_ShowIntNum(70, 100, ds3231_hours, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);

		if (button_count[3] == 1) {
		    ds3231_hours = (ds3231_hours + 1) %24;
		    ds3231_Write(ADDRESS_HOUR, ds3231_hours);
		}
		if (button_count[3] >= 40) flag = 1;
		else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;

			ds3231_hours = (ds3231_hours + 1) %24;
			ds3231_Write(ADDRESS_HOUR, ds3231_hours);
		}
		if(button_count[0] == 1) {TMP = min; count_led_flash = 0; displayAdjTime(); }
		break;
	case min:
		if(count_led_flash < 10){
			lcd_ShowIntNum(110, 100, ds3231_min, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_min = (ds3231_min + 1) % 60;
			ds3231_Write(ADDRESS_MIN, ds3231_min);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			ds3231_min = (ds3231_min + 1) % 60;
			ds3231_Write(ADDRESS_MIN, ds3231_min);
		}
		if(button_count[0] == 1) {TMP = sec; count_led_flash = 0; displayAdjTime();}
		break;
	case sec:
		if(count_led_flash < 10){
			lcd_ShowIntNum(150, 100, ds3231_sec, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_sec = (ds3231_sec + 1) % 60;
			ds3231_Write(ADDRESS_SEC, ds3231_sec);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;

			ds3231_sec = (ds3231_sec + 1) % 60;
			ds3231_Write(ADDRESS_SEC, ds3231_sec);
		}
		if(button_count[0] == 1) {TMP = day; count_led_flash = 0; displayAdjTime();}
		break;
	case day:
		if(count_led_flash < 10){
			lcd_ShowIntNum(20, 130, ds3231_day, 2, BLACK, BLACK, 24);
			lcd_ShowIntNum(70, 130, ds3231_date, 2, BLACK, BLACK, 24);
		} else {
			lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
			lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
		}
		if(button_count[3] == 1) {
			ds3231_day = (ds3231_day % 7) + 1;
			ds3231_date = (ds3231_date % 30) + 1;
			ds3231_Write(ADDRESS_DAY, ds3231_day + 2);
			ds3231_Write(ADDRESS_DATE, ds3231_date);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			ds3231_day = (ds3231_day % 7) + 1;
			ds3231_date = (ds3231_date % 30) + 1;
			ds3231_Write(ADDRESS_DAY, ds3231_day + 2);
			ds3231_Write(ADDRESS_DATE, ds3231_date);
		}
		if(button_count[0] == 1) {TMP = month; count_led_flash = 0; displayAdjTime();}
		break;
	case month:
		if(count_led_flash < 10){
			lcd_ShowIntNum(110, 130, ds3231_month, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_month = (ds3231_month % 12) +  1;
			ds3231_Write(ADDRESS_MONTH, ds3231_month);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			ds3231_month = (ds3231_month % 12) +  1;
			ds3231_Write(ADDRESS_MONTH, ds3231_month);
		}
		if(button_count[0] == 1) {TMP = year; count_led_flash = 0; displayAdjTime();}
		break;
	case year:
		if(count_led_flash < 10){
			lcd_ShowIntNum(150, 130, ds3231_year, 2, BLACK, BLACK, 24);
		} else lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
		if(button_count[3] == 1) {
			ds3231_year = (ds3231_year % 99) +  1;
			ds3231_Write(ADDRESS_YEAR, ds3231_year);
		}
		if (button_count[3] >= 40) flag = 1; else flag = 0;
		if (button_count[3] % 4 == 1 && flag == 1) {
			count_led_flash = 11;
			ds3231_year = (ds3231_year % 99) +  1;
			ds3231_Write(ADDRESS_YEAR, ds3231_year);
		}
		if(button_count[0] == 1) {TMP = hour; count_led_flash = 0; displayAdjTime();}
		break;
	default:
		break;
	}
}

void  fsm_run(){
	switch(status){
	case INIT:
		updateTime();
//		testAlarm();       setup for arlam testing
		status = MODE1;
		break;
	case MODE1: //done and running
		alarm();
		if(button_count[14] == 1) {
			lcd_Clear(BLACK);
			status = MODE2;
			TMP = hour;
			displayAdjTime();
		}
		break;
	case MODE2: //done and running
		adjustingTime_mode();
		if(button_count[14] == 1) {
			status = MODE3;
			lcd_Clear(BLACK);
			TIMER = init;
		}
		break;
	case MODE3: //done and running
		timer_mode();
		if(button_count[14] == 1) {
			status = MODE1;
			lcd_Clear(BLACK);
			clockTime();
		}
		break;
	default:
		break;
	}
}
