/**
 * sectorobject.c
 * -------------------
 * To sector management object.
 *
 */

// include Files
#include "sectorobject.h"

Sectorobj* newsectorobject(Sectorobj* sectobj, long num, unsigned char secttype, long* now){
	Sectorobj* ret;

	ret = (Sectorobj*)realloc(sectobj, sizeof(Sectorobj)*num);
	if(ret == NULL){
		return ret;
	}

	// init new sector
	ret[num-1].sectortype = secttype;
	ret[num-1].starttime = 0;
	ret[num-1].correcttime = 0;
	ret[num-1].resttime = 0;
	ret[num-1].finaltime = 0;
	ret[num-1].average = 0;
	ret[num-1].distance = 0;
	ret[num-1].nowtime = now;

	return ret;
}

void sectortimecalc(Sectorobj* sectobj){
	switch(sectobj->sectortype){
		case AVERAGESECTOR:{
			if(sectobj->average == 0){return;}	// 0div
			// calc correcttime
			sectobj->correcttime = sectobj->starttime + (sectobj->distance/sectobj->average) + sectobj->resttime;
			if(sectobj->correcttime >= DAYTIME){
				sectobj->correcttime -= DAYTIME;
			}
			// calc finaltime
			sectobj->finaltime = sectobj->correcttime - *sectobj->nowtime;
			if(sectobj->finaltime > (DAYTIME/2)){
				sectobj->finaltime -= DAYTIME;
			}else if(sectobj->finaltime < (DAYTIME/-2)){
				sectobj->finaltime += DAYTIME;
			}
			break;
		}
		case TCSECTOR:{
			break;
		}
		case SSSECTOR:{
			break;
		}
	}
}