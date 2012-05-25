#ifndef _NORTH_H_
#define _NORTH_H_

	#include <avr/interrupt.h>
	#include <util/delay.h>
	#include <buffer.h>
	#include "HD44780.h"
	#include "south.h"
	
	#define LCD_RW PB5
	
	/**
	 * pin zgłaszania przerwania klawiatury
	 * */
	#define HWB PE2 
	#define hwb ((PINE & _BV(HWB)) >> HWB)

	#define SCL PD0 
	#define SDA PD1 
	#define scl ((PIND & _BV(SCL)) >> SCL)
	#define sda ((PIND & _BV(SDA)) >> SDA)
	
	/**
	 * właczenie buforowania klawiatury
	 * */
	#define north_buffer_on ;
	
	/**
	 * wyłaczenie buforowania klawiatury
	 * */
	#define north_buffer_off ;
	
	/**
	 * czas 
	 * */
	long north_time;
	
	/**
	 * bufor klawiatury
	 * */
	cBuffer * north_buffer;
	
	/**
	 * inicjalizacja
	 * */
	void north_init (void);
	
	/**
	 * testy
	 * */
	void north_test (void);
	
#endif /* _NORTH_H_ */
