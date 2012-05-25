#ifndef _CONF_H_
#define _CONF_H_

	#define conf_init conf_read
	
	typedef struct Conf
	{
		char n;
		char m;
		char time_min;
		char time_max;
	
	} Conf;
	
	Conf conf; 
	
	/**
	 * odczyt konfiguracji
	 * */
	void conf_read (void);
	/**
	 * zapis konfiguracji
	 * */
	void conf_write (void);
	
#endif /* _CONF_H_ */
