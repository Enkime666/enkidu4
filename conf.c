#include "conf.h"
	#include <avr/eeprom.h>


uint32_t EEMEM n = 1 ; 
uint32_t EEMEM m = 1 ; 
uint32_t EEMEM time_min = 1 ; 
uint32_t EEMEM time_max = 2 ; 


void conf_read (void)
{
	conf.n = eeprom_read_dword (n);
	conf.m = eeprom_read_dword (m);
	conf.time_min = eeprom_read_dword (time_min);
	conf.time_max = eeprom_read_dword (time_max);
}

void conf_write (void)
{
	eeprom_update_dword (n, conf.n);
	eeprom_update_dword (m, conf.m);
	eeprom_update_dword (time_min, conf.time_min);
	eeprom_update_dword (time_max, conf.time_max);
}
