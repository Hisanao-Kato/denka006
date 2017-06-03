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
HardwareObject *hwobj;

//debug value
long nowtime;
long testval1;
long testval2;

int main(void){
	// init
	char initmode = returninitmode();
	hwobj = hardwareinit();


	//test
	hwobj->mainbuf_i[0] = 'A';
	hwobj->mainbuf_i[1] = 'B';
	setbeep(BP_ACK);

	Sectorobj* sectobj = (Sectorobj*)calloc(1, sizeof(Sectorobj));
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

	if(counter > 9){
		nowtime++;
		if(nowtime >= DAY){nowtime -= DAY;}
		counter = 0;
	}
	// debug
	testval1++;
	testval2+=100;
	makedisp(nowtime, MAIN_FIRST, V_DBG);
	makedisp(testval1, MAIN_SECOND, V_DBG);
	makedisp(testval2, MAIN_THIRD, V_DBG);


	// Calc Time

	// View Output

}
