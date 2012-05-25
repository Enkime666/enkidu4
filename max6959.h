#ifndef _MAX6959_H_
#define _MAX6959_H_

	#include <inttypes.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <avr/io.h>
	#include <util/delay.h>
	#include <util/twi.h>
	#include <i2c.h>

	typedef struct SDigits {
		uint8_t digit0;
		uint8_t digit1;
		uint8_t digit2;
		uint8_t digit3;
	} SDigits;

	typedef union UDigits {
		uint32_t digits32;
		SDigits str;
	} UDigits;

	/**
	 * adresy cząstkowe
	 * */
	#define TWI_SLA_MAX6959 0b01110000
	#define MAX695_A 0b00
	#define MAX695_B 0b10

	/**
	 * komendy (rozkazy)
	 * */
	#define NO_OP 0x00
	#define DECODE_MODE 0x01
	#define INTENSITY 0x02
	#define SCAN_LIMIT 0x03
	#define CONFIGURATION 0x04
	#define FACTORY_RESERVED 0x05
	#define GPIO 0x06
	#define DISPLAY_TEST 0x07
	#define READ_KEY_DEBOUNCED 0x08
	#define READ_KEY_PRESSED 0x0C
	#define DIGIT_0 0x20
	#define DIGIT_1 0x21
	#define DIGIT_2 0x22
	#define DIGIT_3 0x23
	#define SEGMENTS 0x24

	/** 
	 * test segmentów
	 * */
	void max6959_test (unsigned char ab);
	
	//int max6959_write_commend (unsigned char, uint8_t);
	//int max6959_write_commend_and_byte (unsigned char, uint8_t, uint8_t);
	/**
	 * zapis komendy i danych do sterownika 
	 * @param command komenda
	 * @param data tablica danych
	 * @param len długość danych
	 * */
	int max6959_write_data (unsigned char, uint8_t command, uint8_t *data, int len);
	/**
	 * zapis komendy i odczyt danych do sterownika 
	 * @param command komenda
	 * @param data tablica danych
	 * @param len długość danych
	 * */
	int max6959_read_data (unsigned char, uint8_t* command, uint8_t *data, int len);
	
	#define max6959_write_command(ab, command) max6959_write_data (ab, command, 0,0)
	#define max6959_write_command_data(ab, command, data) max6959_write_data (ab, command, &data,1)
	
	//int max6959_write_number (unsigned char, UDigits);
	
	/**
	 * zapis surowych danych do max6959
	 * */
	uint8_t max6959_write (char ab, uint8_t command, uint8_t data);
	/**
	 * odczyt surowych danych z max6959
	 * */
	uint8_t max6959_read (char ab, uint8_t command, uint8_t *data);
	/**
	 * zapis liczby do sterownika
	 * @param number liczba
	 * @param base podstawa
	 * @param a pozycja pierwszej cyfry
	 * @param b pozycja o jeden większa od ostatniej cyfry
	 * */
	void max6959_write_number (unsigned char ab, uint16_t number, char base, char a, char b);
	/**
	 * zapis liczby czterofycrowej
	 * */
	#define max6959_write_number_x(ab,number,base) max6959_write_number (ab, number, base, 0, 4)
	/**
	 * zapis liczby dwucyfrowej na starszą pozycję
	 * */
	#define max6959_write_number_l(ab,number,base) max6959_write_number (ab, number, base, 0, 2)
	/**
	 * zapis liczby dwucyfrowej na niższą pozycję
	 * */
	#define max6959_write_number_h(ab,number,base) max6959_write_number (ab, number, base, 2, 4)

	

#endif /* _MAX6959_H_ */
