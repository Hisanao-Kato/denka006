/**
 * sectorobject.h
 * -------------------
 * Sector management object header file.
 *
 */

#include <stdlib.h>

#define AVERAGESECTOR 0
#define TCSECTOR 1
#define SSSECTOR 2

#define DAYTIME 864000	//TODO:どっちにすっかなぁ

typedef struct{
	unsigned char sectortype;
	unsigned long starttime;
	unsigned long correcttime;
	long resttime;
	long finaltime;
	double average;
	long distance;
	long* nowtime;
}Sectorobj;

// define public function
Sectorobj* newsectorobject(Sectorobj* sectobj, long num, unsigned char secttype, long* now);
void sectortimecalc(Sectorobj* sectobj);
