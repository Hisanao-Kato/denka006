#ifndef DENKAHW_H
#define DENKAHW_H
/**
 * denkahw.h
 * -------------------
 * Denka hardware header file.
 *
 */

// include Files
#include <stdlib.h>
#include "p24FJ64GA004.h"

#define SUCCESS	0
#define FAILED	-1

#define INITMODE_AVERAGE 0
#define INITMODE_TC 1
#define INITMODE_OFFICIAL 2
#define INITMODE_VERSION 3
#define INITMODE_USER 4
#define INITMODE_DEBUG 5

#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_F 10
#define KEY_P 11
#define KEY_N 12
#define KEY_M 13
#define KEY_D 14
#define KEY_E 15
#define KEY_CP 16
#define KEY_PC 17
#define KEY_MP 18
#define KEY_CP_MP 19
#define KEY_PC_MP 20
#define DO_NOT_PUSHED_KEY -1

#define MAIN_FIRST 0
#define MAIN_SECOND 1
#define MAIN_THIRD 2
#define EXT_FIRST 3
#define EXT_SECOND 4
#define EXT_THIRD 5

#define V_CLOCK 0
#define V_TIME 1
#define V_DIST 2
#define V_REST 3
#define V_DBG 4
#define V_DBGOUT 5

#define BP_LONG 0x7FF
#define BP_ACK 0x3
#define BP_NACK 0x15

#define NTL 0
#define FWD 1
#define REV 2

#define PULSE_4 392464678
#define PULSE_2 784929356
#define PULSE_8 196232339

#define DELAY asm("NOP");

// define object
typedef struct {
	unsigned char mainbuf_i[2];
	unsigned char mainbuf_f[6];
	unsigned char mainbuf_s[6];
	unsigned char mainbuf_t[6];
	unsigned char extbuf_f[6];
	unsigned char extbuf_s[6];
	unsigned char extbuf_t[6];

	unsigned char keystat;
	unsigned char tripvector;
	double distcalib;
}HardwareObject;

// function prototiping
HardwareObject* hardwareinit(void);
char returninitmode(void);
void makedisp(long value, char whitch, char changetype);
void setbeep(long beeptype);
long getadddistance(void);
void settripvector(unsigned char tripvect);
void calctripcalib(long officialtrip, long selftrip);
void setpulse(long value);
long gethwadddist(void);

#endif
