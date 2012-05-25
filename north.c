#include "north.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
#include <mmc.h>
#include "fat.h"

/* przerwania */

/** 
 * przerwanie dla przycisków
 * */
ISR (INT6_vect)
{
	north_read_key_pressed ();
}
/** 
 * przerwanie dla licznika czasu
 * */
ISR (TIMER3_COMPA_vect)
{
	++north_time;
}

/* private */
/**
 * odczyt wciśniętych przycisków
 * */
void north_read_key_pressed (void)
{
	uint32_t data;
	char i;
	data = south_up (READ_KEY_PRESSED);
	for (i = 0; i < 32; ++i)
	{
		if (data & _BV (i)) 
		{
			bufferAddToEnd (north_buffer, (hwb & _BV (7)) | ((north_buffer, PIND & 0b11000000) >> 1) | i);
		}
	}
}

/* public */
/**
 * odczyt blokujacy jednego znaku z klawiatury
 * */
int	north_getchar (char c, FILE *stream)
{
	for (;;)
	{
		if (bufferIsNotEmpty (north_buffer)) {
			return bufferGetFromFront (north_buffer);
		}
		north_read_key_pressed ();
	}	
}

FILE south_str = FDEV_SETUP_STREAM(NULL , north_getchar, _FDEV_SETUP_READ);
//FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
//FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
//FILE usb_str = FDEV_SETUP_STREAM(usb_putchar, NULL, _FDEV_SETUP_WRITE);

void north_init (void)
{
	stdin = &south_str;
	//stdout = &lcd_str;
	//stderr = &

	timer3_init ();
	int6_init ();
	LCD_Initalize ();
	south_init ();
	//mmcInit();
	//bufferInit (north_buffer, NULL, 32);
}

/* private */

/**
 * inicjalizacja przerwania klawiatury
 * */
void int6_init (void)
{
	cli ();
	EICRB &= ~(_BV (ISC61) | _BV (ISC60));
	//EIMSK &= ~_BV (INT6);
	sei ();
}
/** 
 * inicjalizacja przerwania timera czasu
 * */
void timer3_init (void)
{
	cli ();
	TCCR3B |= _BV (WGM32) | _BV (CS32); //ustawiam preselekter
	OCR3A = 3125;	//ustawiam wartość
	TIMSK3 |= _BV (OCIE3A); //odblokowywuje przerwanie
	sei ();
}

void north_test (void) 
{
	//south_test ();
	north_test_south ();
}
/**
 * testuje komunikacje i wyświetla ekran z danymi
 * */
void north_test_south (void) 
{
	char ptr[4];
	char i;
	uint8_t stat, data = 0;
	for (i = 0; i < 4; ++i)
	{
		stat = max6959_write (south_max6959 (i), INTENSITY, 0x3f);
		LCD_GoTo (0, i);
		
		if (stat == I2C_OK)
		{
			//LCD_WriteText ("w: 0x3f r: ");
			LCD_WriteText (utoa (TWI_SLA_MAX6959| south_max6959 (i) | abc, ptr, 16));
			LCD_WriteText (" ");
		}
		else 
		{
			LCD_WriteText ("wtf ");
		}
			
		stat = max6959_read (south_max6959 (i), INTENSITY, &data);
		if (stat == I2C_OK)
		{
			LCD_WriteText (" r: " );
			LCD_WriteText (utoa (data, ptr, 16));
		}
		else 
		{
			LCD_WriteText (" e: " );
			LCD_WriteText (utoa (stat, ptr, 16));
		}
		stat = max6959_read (south_max6959 (i), SCAN_LIMIT, &data);
		if (stat == I2C_OK)
		{
			LCD_WriteText (" r: " );
			LCD_WriteText (utoa (data, ptr, 16));
		}
		else 
		{
			LCD_WriteText (" e: " );
			LCD_WriteText (utoa (stat, ptr, 16));
		}
	}
	getchar ();
}


