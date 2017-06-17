/**
 * denkahw.c
 * -------------------
 * Denka hardware source file.
 *
 */

// include Files
#include "denkahw.h"
#include <stdlib.h>

// define params
#define TENKEY1     PORTBbits.RB0
#define TENKEY2     PORTBbits.RB1
#define TENKEY3     PORTBbits.RB2
#define TENKEY4     PORTBbits.RB3
#define TENKEY5     PORTAbits.RA3
#define TENKEY6     PORTAbits.RA8
#define TENKEY7     PORTBbits.RB4
#define TENKEY8     PORTAbits.RA9
#define CPBTN       PORTCbits.RC6
#define PCBTN       PORTCbits.RC7
#define MPBTN       PORTCbits.RC8
#define BACK        PORTBbits.RB7
#define SPEED       PORTBbits.RB6
#define BEEP        PORTCbits.RC9
#define MDSPSTT		LATAbits.LATA10
#define MDSPDT		LATB
#define MDSPCLK		LATAbits.LATA7

#define PUSHED 0
#define ON 1
#define OFF 0


// function prototiping
void outputmaindisp(void);
void outputbeep(void);
void delay_us(long usec);
void checkkey(void);

// define struct
typedef struct{
	unsigned long beeptable;
	unsigned long beepcounter;
}Beepobject;

// private value
HardwareObject* hw;
Beepobject *bp;
long hwadddist;
char trpvect[3][2] = {{NTL,NTL},{FWD,REV},{REV,FWD}};
unsigned long pulsedist;

/**
 * Timer3 Interrupt
 */
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
	static char counter = 0;
	IFS0bits.T3IF = 0;
	counter++;

	//return; //debug
	delay_us(1);
	delay_us(100);
	delay_us(257);
	delay_us(10000);
	// inputkey
	checkkey();
	// outputbeep
	outputbeep();
	// mainproc
	main_timerproc();


	if(counter > 9){
		//outputmaindisp
		outputmaindisp();
		counter = 0;
	}


}

/**
 * Int1 Interrupt
 */
void  __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void){

}

/**
 * hardwareinit
 */
HardwareObject* hardwareinit(void){
	/* Clock init */
	CLKDIV = 0;

	/* Port setting input=1 */
	TRISA = 0b0000001100001001;
	TRISB = 0b0000000011010000;
	TRISC = 0b0000000111000000;

	/* Timer setting */
	PR2 = 0b1111101111001111;		// 26MHz(2clk=1cycle) 13MCycle -> 10ms (1/100) -> 12999
	PR3 = 0b1;
	//PR2 = 0b0110100111111111;
	//PR3 = 0b11000;
	IPC2bits.T3IP = 4;
	T2CON = 0b0000000000001000;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;

	/*  */
	RPINR0bits.INT1R = 6;
	INTCON2bits.INT1EP = 1;
	IPC5bits.INT1IP = 3;
	IFS1bits.INT1IF = 0;
	IEC1bits.INT1IE = 1;

	/*  */
	CMCON = 0;
	AD1PCFG = 0b1111111111111110;
	AD1CON1 = 0b0000000011100000;
	AD1CON2 = 0b0000000000000000;
	AD1CON3 = 0x1F05;
	AD1CSSL = 0;

	T2CONbits.TON = 1; // Timer on

	hw = (HardwareObject*)calloc(1, sizeof(HardwareObject));
	bp = (Beepobject*)calloc(1, sizeof(Beepobject));

	hw->distcalib = 1;
	pulsedist = PULSE_4;

    return hw;
}

/**
 * returninitmode
 */
char returninitmode(void){
	if(CPBTN == PUSHED){
		if(PCBTN == PUSHED && MPBTN == PUSHED){
			return INITMODE_DEBUG;
		}else{
			return INITMODE_TC;
		}
	}
	if(PCBTN == PUSHED){
		return INITMODE_OFFICIAL;
	}
	if(MPBTN == PUSHED){
		return INITMODE_USER;
	}
	return INITMODE_AVERAGE;
}

/**
 * Display関係
 */
void outputmaindisp(void){
	unsigned char outbuf[21];
	int i;
	outbuf[0] = 0x00;	//TODO:設定
	outbuf[1] = hw->mainbuf_s;
	outbuf[7] = hw->mainbuf_t;
	outbuf[13] = hw->mainbuf_f;
	outbuf[19] = hw->mainbuf_i;

	MDSPSTT = ON;
	delay_us(10);
	for(i=0; i<21; i++){
		MDSPDT = (MDSPDT & 0xFF) | ((int)(outbuf[i]) << 8 & 0xFF00);
		MDSPCLK = ON;
		DELAY; DELAY; DELAY; DELAY; DELAY;
		MDSPCLK = OFF;
	}
	MDSPSTT = OFF;
}

void makedisp(long value, char whitch, char changetype){
	unsigned char* target;
	switch(whitch){
		case MAIN_FIRST:{
			target = hw->mainbuf_f;
			break;
		}
		case MAIN_SECOND:{
			target = hw->mainbuf_s;
			break;
		}
		case MAIN_THIRD:{
			target = hw->mainbuf_t;
			break;
		}
		case EXT_FIRST:{
			target = hw->extbuf_f;
			break;
		}
		case EXT_SECOND:{
			break;
		}
		case EXT_THIRD:{
			break;
		}
	}
	switch(changetype){
		case V_CLOCK:{
			target[0] = (value / 360000) + 0xA0;
			target[1] = ((value % 360000) / 36000) + 0xD0;
			target[2] = ((value % 36000) / 6000) + 0xA0;
			target[3] = ((value % 6000) / 600) + 0xD0;
			target[4] = ((value % 600) / 100) + 0xA0;
			target[5] = ((value % 100) / 10) + 0xA0;
			break;
		}
		case V_TIME:{
			if(value < 0){
				value *= -1;
				target[0] = '-';
			}
			target[1] = ((value % 36000) / 6000) + 0xA0;
			target[2] = ((value % 6000) / 600) + 0xD0;
			target[3] = ((value % 600) / 100) + 0xA0;
			target[4] = ((value % 100) / 10) + 0xD0;
			target[5] = (value % 10) + 0xA0;
			break;
		}
		case V_DIST:{
			break;
		}
		case V_REST:{
			break;
		}
		case V_DBG:{
			target[0] = (value / 100000);
			target[1] = ((value % 100000) / 10000);
			target[2] = ((value % 10000) / 1000);
			target[3] = ((value % 1000) / 100);
			target[4] = ((value % 100) / 10);
			target[5] = (value % 10);
			break;
		}
		case V_DBGOUT:{
			target[0] = (value / 100000) + 0xA0;
			target[1] = ((value % 100000) / 10000) + 0xA0;
			target[2] = ((value % 10000) / 1000) + 0xA0;
			target[3] = ((value % 1000) / 100) + 0xA0;
			target[4] = ((value % 100) / 10) + 0xA0;
			target[5] = (value % 10) + 0xA0;
			break;
		}
	}
}



/**
 * Beep関係
 */
void setbeep(long beeptype){
	if(bp->beepcounter >= 32){
		return;
	}
	bp->beepcounter = 0;
	bp->beeptable = beeptype;
}
void outputbeep(void){
	if(bp->beepcounter >= 32){
		bp->beepcounter = 32;
		BEEP = 0;
		return;
	}
	BEEP = bp->beeptable >> bp->beepcounter & 0x1;
	bp->beepcounter++;
	delay_us(1500);
	BEEP = bp->beeptable >> bp->beepcounter & 0x1;
	bp->beepcounter++;
}

void checkkey(void){
	hw->keystat = DO_NOT_PUSHED_KEY;

	// CP PC MAP
	if(CPBTN == PUSHED){
		if(MPBTN == PUSHED){
			hw->keystat = KEY_CP_MP;
		}else{
			hw->keystat = KEY_CP;
		}
	}else if(PCBTN == PUSHED){
		if(MPBTN == PUSHED){
			hw->keystat = KEY_PC_MP;
		}else{
			hw->keystat = KEY_PC;
		}
	}else if(MPBTN == PUSHED){
		hw->keystat = KEY_MP;
	}

	if(hw->keystat != DO_NOT_PUSHED_KEY){
		return;
	}

	// Tenkey
	TENKEY1 = TENKEY2 = TENKEY3 = TENKEY4 = OFF;
	while(TENKEY5 == ON || TENKEY6 == ON || TENKEY7 == ON || TENKEY8 == ON);

	TENKEY4 = ON;
	delay_us(100);
	if(TENKEY8 == ON){hw->keystat = KEY_E;}else
	if(TENKEY7 == ON){hw->keystat = KEY_D;}else
	if(TENKEY6 == ON){hw->keystat = KEY_0;}else
	if(TENKEY5 == ON){hw->keystat = KEY_M;}
	TENKEY4 = OFF;
	while(TENKEY5 == ON || TENKEY6 == ON || TENKEY7 == ON || TENKEY8 == ON);

	TENKEY3 = ON;
	delay_us(100);
	if(TENKEY8 == ON){hw->keystat = KEY_3;}else
	if(TENKEY7 == ON){hw->keystat = KEY_2;}else
	if(TENKEY6 == ON){hw->keystat = KEY_1;}else
	if(TENKEY5 == ON){hw->keystat = KEY_N;}
	TENKEY3 = OFF;
	while(TENKEY5 == ON || TENKEY6 == ON || TENKEY7 == ON || TENKEY8 == ON);

	TENKEY2 = ON;
	delay_us(100);
	if(TENKEY8 == ON){hw->keystat = KEY_6;}else
	if(TENKEY7 == ON){hw->keystat = KEY_5;}else
	if(TENKEY6 == ON){hw->keystat = KEY_4;}else
	if(TENKEY5 == ON){hw->keystat = KEY_P;}
	TENKEY2 = OFF;
	while(TENKEY5 == ON || TENKEY6 == ON || TENKEY7 == ON || TENKEY8 == ON);

	TENKEY1 = ON;
	delay_us(100);
	if(TENKEY8 == ON){hw->keystat = KEY_9;}else
	if(TENKEY7 == ON){hw->keystat = KEY_8;}else
	if(TENKEY6 == ON){hw->keystat = KEY_7;}else
	if(TENKEY5 == ON){hw->keystat = KEY_F;}
	TENKEY1 = OFF;
	while(TENKEY5 == ON || TENKEY6 == ON || TENKEY7 == ON || TENKEY8 == ON);

}

/**
 * Distance関係
 */
void calc_dist(void){

	static signed long distbuf = 0;

	delay_us(13);	if(SPEED == 1){return;}
	delay_us(13);	if(SPEED == 1){return;}
	delay_us(26);	if(SPEED == 1){return;}
	delay_us(54);	if(SPEED == 1){return;}
	delay_us(108);	if(SPEED == 1){return;}
	delay_us(216);	if(SPEED == 1){return;}
	delay_us(30);	if(SPEED == 1){return;}

	switch(trpvect[hw->tripvector][BACK]){
		case NTL:{break;}
		case FWD:{
			distbuf += (long)(pulsedist / hw->distcalib);
			break;
		}
		case REV:{
			distbuf -= (long)(pulsedist / hw->distcalib);
			break;
		}
		default:{break;}
	}
	if(distbuf != 0){
		hwadddist += distbuf / 1000000000;
		distbuf	%= 1000000000;
	}
}

long gethwadddist(void){
	long ret = hwadddist;
	hwadddist = 0;
	return ret;
}

void settripvector(unsigned char tripvect){
	hw->tripvector = tripvect;
}

void calctripcalib(long officialtrip, long selftrip){
	hw->distcalib = officialtrip / selftrip;
}

void setpulse(long value){
	pulsedist = value;
}
/**
 * delay_us
 */
void delay_us(long usec){
	while(usec){
		DELAY;
		usec--;
	}
}
