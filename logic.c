#include "logic.h"
#include <string.h>
#include "north.h"
#include "conf.h"
#include "stat.h"

/* private */
/**
 * konfiguracja 
 * */
void logic_conf (void)
{
	char buf[4][4]; /* bufor na edytowane dane */
	char key; /* pobrany znak */
	short i; /*  */
	short w=0, x=0, y=0, z=0;
	
	/* konwersje liczb na stringi */
	itoa (conf.n, buf[0], 10);
	itoa (conf.m, buf[1], 10);
	itoa (conf.time_min, buf[2], 10);
	itoa (conf.time_min, buf[3], 10);

	/* głowna petla konfiguracji */
	for (;;) 
	{

	LCD_Clear ();
	
	LCD_GoTo_WriteText (0, 0, "n : ");
	LCD_GoTo_WriteText (0, 1, "m : ");
	LCD_GoTo_WriteText (0, 2, "minimal time : ");
	LCD_GoTo_WriteText (0, 3, "maximal time : ");

	LCD_GoTo_WriteText (15, 0, buf[0]);
	LCD_GoTo_WriteText (15, 1, buf[1]);
	LCD_GoTo_WriteText (15, 2, buf[2]);
	LCD_GoTo_WriteText (15, 3, buf[3]);

	key = getchar();
	
	switch (key)
	{
		case ':' : 
			/* save ediotion to conf */
			if (z)
			{
				i = atoi (buf[y]);
				if (i)
				{
					switch (y)
					{
						case '0' : conf.n = i; break;
						case '1' : conf.m = i; break;
						case '2' : conf.time_min = i; break;
						case '3' : conf.time_max = i; break;
					}
					z = 0;
				}
			}
			/* go into edition */
			else z = 1;
			break;
		case ';' :
			/* go out from edidion */
			if (z) z = 0;
			/* go out from config */
			else
			{
				conf_write ();
				return;
			}
			break;
		case '<' : 
			if (z) 
			{
				if (x) --x;
			}
			else y = (y+3)%4;
			break;
		case '=' : /* insert */
			w = !w;
			break;
		case '>' : 
			if (z) 
			{
				if (x < strlen (buf[y])) ++x;
			}
			else y = (y+1)%4;
			break;
		case '?' : /* delete */
			if (z && strlen (buf[y])) 
			{
				for (i = x; i < strlen (buf[y]);++i)
				{
					buf[y][i] = buf[y][i+1];
				}
				buf[y][i+1] = 0;
			}	
			break;
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' :
			if (w) 
			{
				buf[y][x] = key;
				++x;
			}
			else 
			{
				for (i = strlen (buf[y]); i > x; --i)
				{
					buf[y][i] = buf[y][i-1];
				}
				buf[y][x] = key;
			}
			break;
		}
	}
}
/**
 * gra właściwa
 * */
void logic_game (void)
{
	char str[16]; 
	Stat stat;
	long time_temp, time_start;
	uint32_t data = 0, data_new = 0;
	int i;
	char j;

	stat.n = conf.n;
	stat.m = conf.m;
	stat.time_sum = 0;
	
	north_buffer_off;
	for (i = 0; i < stat.n; ++i)
	{
		//_delay_ms (rand() % (conf.time_max-conf.time_min) + conf.time_min);
		time_start = north_time;
		
		/* losowanie nowych punktów */
		for (j = 0; j < stat.m;)
		{
			data_new = _BV (rand () & 0b1111 );
			if (!(data ^ data_new))
			{
				data |= data_new;
				++j;
			}
		}
		
		/* wyświetlenie aktualne stanu i sprawdzenie przycisków*/
		do
		{
			LCD_Clear ();
			LCD_GoTo (0, 0);
			LCD_WriteText ("round : ");
			LCD_WriteText (itoa (i, str, 10));
			max6959_write_number_h (south_max6959(0), i, 10);
			
			LCD_WriteText ("from : ");
			LCD_WriteText (itoa (conf.n, str, 10));
			max6959_write_number_l (south_max6959(0), stat.n, 10);
			
			LCD_GoTo (0, 1);
			LCD_WriteText ("time rund : ");
			time_temp = north_time - time_start;
			LCD_WriteText (ltoa (time_temp, str, 10));
			max6959_write_number_l (south_max6959(1), time_temp, 10);

			LCD_GoTo (0, 2);
			LCD_WriteText ("time sum  : ");
			time_temp += stat.time_sum;
			LCD_WriteText (ltoa (time_temp , str, 10));
			max6959_write_number_l (south_max6959(2), time_temp, 10);
			
			LCD_GoTo (0, 3);
			LCD_WriteText ("time ave  : ");
			time_temp = stat.time_sum / (stat.n * stat.m);
			LCD_WriteText (ltoa (time_temp , str, 10));
			max6959_write_number_l (south_max6959(3), time_temp, 10);
			
			south_read (READ_KEY_PRESSED, data_new);
			data &= ~data_new;
		}
		while (!data);
		stat.time_sum +=(north_time - time_start);
	}
	north_buffer_on;
	stat_append (stat);
}	
/**
 * statystyki
 * */
void logic_stat (void)
{
}
/**
 * test klawiatury
 * */
void logic_keyboard_south (void)
{
	uint32_t data;
	char i;
	for (;;)
	{
		data = south_up (READ_KEY_PRESSED);
		for (i = 0; i < 32 ; ++i)
		{
			if  (data & _BV(i))
			{
				LCD_WriteData ( 64 + i);
			}
		}
	}
}

/* public */
void logic_test (void)
{
	north_test ();
}
void logic_run (void)
{
	//logic_keyboard_south ();
	for (;;)
	{
		LCD_Clear ();
	
		LCD_GoTo (0, 0);
		LCD_WriteText ("Menu:");
	
		LCD_GoTo (0, 1);
		LCD_WriteText ("1: Game");

		LCD_GoTo (0, 2);
		LCD_WriteText ("2: Config");
		
		LCD_GoTo (0, 3);
		LCD_WriteText ("3: Stat");
		
		switch (getchar ())
		{
			case '1' : 
				logic_game ();
				break;
			case '2' : 
				logic_conf ();
				break;
			case '3' : 
				logic_stat ();
				break;
		}
	}
}
