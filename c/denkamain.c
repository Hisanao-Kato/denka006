/**
 * denkamain.c
 * -------------------
 * Denka main source file.
 *
 */

// Include Files
#include "denkamain.h"
#include "denkahw.h"
#include "sectorobject.h"
#include <stdlib.h>

#define DAY 864000

// function prototiping

// private values

// private values
HardwareObject* hwobj;
Sectorobj* sectobj;

long sectcounter;
long checkcounter;

//debug value
long nowtime;
long testval1;
long testval2;

int main(void){
	// init
	char initmode = returninitmode();
	hwobj = hardwareinit();

	sectcounter = 1;
	checkcounter = 1;

	switch(initmode){
		case INITMODE_AVERAGE:{
			sectobj = newsectorobject(NULL, sectcounter, AVERAGESECTOR, &nowtime);
			break;
		}
		case INITMODE_TC:{
			break;
		}
		case INITMODE_OFFICIAL:{
			break;
		}
	}


	//test
	hwobj->mainbuf_i[0] = 'A';
	hwobj->mainbuf_i[1] = 'B';
	setbeep(BP_ACK);

	sectobj = (Sectorobj*)calloc(1, sizeof(Sectorobj));
	sectobj = newsectorobject(sectobj, 1, AVERAGESECTOR, &nowtime);

	nowtime = 0;
	sectobj[0].starttime = 1;
	sectobj[0].average = 10;
	sectobj[0].distance = 0;

	sectortimecalc(&sectobj[0]);
	DELAY;

	nowtime = 863999;
	sectobj[0].starttime = 0;
	sectobj[0].average = 10;
	sectobj[0].distance = 0;

	sectortimecalc(&sectobj[0]);
	DELAY;

	nowtime = 0;
	sectobj[0].starttime = 863999;
	sectobj[0].average = 10;
	sectobj[0].distance = 0;

	sectortimecalc(&sectobj[0]);
	DELAY;

	nowtime = 1;
	sectobj[0].starttime = 0;
	sectobj[0].average = 10;
	sectobj[0].distance = 100;

	sectortimecalc(&sectobj[0]);
	DELAY;

	// test end

	// for debug roop
	while(1){
	}
}

void main_timerproc(void){
	static char counter = 0;
	counter++;

	// dist update

	// input check
	
	if(counter > 9){
		nowtime++;
		if(nowtime >= DAY){nowtime -= DAY;}
		// calc Time
		sectortimecalc(&sectobj[sectcounter - 1]);

		// display change
		makedisp(nowtime, MAIN_FIRST, V_CLOCK);
		makedisp(sectobj[sectcounter-1].finaltime, MAIN_SECOND, V_TIME);
		makedisp(testval2, MAIN_THIRD, V_DIST);

		counter = 0;
	}
}
