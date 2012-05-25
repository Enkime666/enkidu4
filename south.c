#include "south.h"
/** 
 * inicjalizacja 
 * */
void south_init (void)
{
        south_write_broadcast (INTENSITY, 0x3f);
}


void south_irq (char i)
{
        
}
/**
 * ustawienie odpowiedniego max6959
 * */
char south_max6959 (char i) {
        PORTD &= ~_BV (ABC);
        PORTD |= (i & 2) << (ABC-1);
        return (i & 1) << 1;
}
void south_read (char command, uint8_t *data) 
{ 
        char i;
        for (i = 0; i < 4; ++i)
        {
                max6959_read (south_max6959 (i), command , data+i);
        }
        
}
/**
 * zapis surowych danych do sterownika
 * */
void south_write (char command, uint32_t data) 
{ 
        char i;
        uint8_t *ptr;
        ptr = &data;
        for (i = 0; i < 4; ++i)
        {
                max6959_read (south_max6959 (i), command , ptr[i]);
        }
        
}
/** 
 * zapis rozgłoszeniowy surowych danych do sterownika
 * */
void south_write_broadcast (uint8_t command, uint8_t data)
{
        char i;
        for (i = 0; i < 4; ++i)
        {
                max6959_write (south_max6959 (i), command, data);
        }
}

/**
 * konwersja w górę danych ze sterownika
 * */
uint32_t south_up (uint8_t command)
{
        uint32_t data_down, data_up = 0;        
        south_read (command, &data_down);
        
        data_up |= ((0b1111 << 0) & data_down) << 0;
        data_up |= ((0b1111 << 1) & data_down) << 4;
        data_up |= ((0b1111 << 2) & data_down) << 1;
        data_up |= ((0b1111 << 3) & data_down) << 5;
        data_up |= ((0b1111 << 4) & data_down) << 2;
        data_up |= ((0b1111 << 5) & data_down) << 6;
        data_up |= ((0b1111 << 6) & data_down) << 3;
        data_up |= ((0b1111 << 7) & data_down) << 7;
        return data_up;
}
/**
 * konwersja w dół danych do sterownika
 * */
void south_down (uint8_t command, uint32_t data_up)
{
        uint32_t data_down = 0;
        
        data_down |= ((0b1111 << 0) & data_up) << 0;
        data_down |= ((0b1111 << 0) & data_up) << 2;
        data_down |= ((0b1111 << 0) & data_up) << 4;
        data_down |= ((0b1111 << 0) & data_up) << 6;
        data_down |= ((0b1111 << 0) & data_up) << 1;
        data_down |= ((0b1111 << 0) & data_up) << 3;
        data_down |= ((0b1111 << 0) & data_up) << 5;
        data_down |= ((0b1111 << 0) & data_up) << 7;
        
        south_write (command, data_down);
}

void south_segments_once (char u)
{
        uint16_t w;
        char i;
        w = _BV (u);
        
        for (i = 0; i < 4; ++i)
        {
                max6959_write (south_max6959 (i), SEGMENTS, (w & (0b1111 << (i<<2))) >> (i << 2));
        }
}
/**
 * testy
 * */
void south_test (void)
{
        char i;
        for (i = 0; i < 4; ++i)
        {
                max6959_test (south_max6959 (i));
        }
}
