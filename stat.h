#ifndef _STAT_H_
#define _STAT_H_

	typedef struct Stat
	{
		int time_sum; /* czas sumaryczny */
		short n; /* liczba rund */
		short m; /* liczba punktów w rundzie */
		char * name; /* nazwa zawodnika */
	} Stat;
		
	/**
	 * odczyt statystyk
	 * */	
	Stat stat_read (void);
	
	/**
	 * zapis statystyk
	 * */
	void stat_write (void);
	
	/** 
	 * dopisanie nowej pozycji do statystyk
	 * */
	void stat_append (Stat stat);
	
#endif /* _CONF_H_ */
