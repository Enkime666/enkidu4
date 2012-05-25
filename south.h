#ifndef _SOUTH_H_
#define _SOUTH_H_

	#include "max6959.h"

	/**
	 * pin do wyboru max6959
	 * */
	#define ABC PD4
	#define abc ((PORTD & _BV(ABC)) >> ABC)

	/**
	 * inicjalizacja
	 * */
	void south_init (void);

	/**
	 * test segmentów
	 * */
	void south_test (void);
	
	/**
	 * konwertuje w dół i zapisuje dane
	 * */
	void south_down (uint8_t command, uint32_t data_up);
	
	/**
	 * odczytuje i konwertuje w góre dane
	 * */
	uint32_t south_up (uint8_t command);
	
	/**
	 * odczytanie surowych danych z sterowników
	 * */
	void south_read (char command, uint8_t *data);
	
	/**
	 * zapisuje do wszystkoch taką samą dane
	 * */
	void south_write_broadcast (uint8_t command, uint8_t data);
	
	/**
	 * ustawia odpowiedni max6959
	 * */
	char south_max6959 (char i);

#endif /* _SOUTH_H_ */
