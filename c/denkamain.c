/**
 * denkamain.c
 * -------------------
 * Denka main source file.
 *
 */

// Include Files
#include "denkamain.h"
#include "denkahw.h"
#include <stdlib.h>

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


	// for debug roop
	while(1){
	}
}

void main_timerproc(void){
	static char counter = 0;
	counter++;

	if(counter > 9){
		nowtime++;
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
