#ifndef _LOGIC_H_
#define _LOGIC_H_
	#include "north.h"
	
	/**
	 * inicjalizacja
	 * */
	#define logic_init() north_init()
	
	/** 
	 * test
	 * */
	void logic_test (void);
	
	/**
	 * wywołanie 
	 * */
	void logic_run (void);

#endif /* _LOGIC_H_ */
