/*
@filename:hardware.c
@description:
ハードウェアコントロールを行う。
*/

#include "hardware.h"

typedef struct {
  long beepParamBit;
  char beepBitCount;
}Beepstat;

Beepstat bpst;
//実装
HardwareObject initHardware(void){
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

    HardwareObject retobj = (HardwareObject)calloc(1, sizeof(HardwareObject));
    //retobj = (HardwareObject)calloc(1, sizeof(HardwareObject));
    //if(retobj == NULL){
    //    // Error
    //}

    return retobj;
}
char freeHardware(HardwareObject obj){
    //free(obj);
}

// Public Function
char setBeep(char beeptype){
    // 優先順位はCP/PC>NG>OK
    if(beeptype < beepStat->beepParamBit){
        return FALSE;
    }else{
        beepstat->beepParamBit = beeptype;
        beepstat->beepBitCount = 0;
        return TRUE;
    }
}

char getKeyValue(void){

}

char getWakeupMode(void){
    if(CPBTN == PUSHED && PCBTN == PUSHED && MPBTN == PUSHED){
        return VERSIONCHECKMODE;
    }
    if(CPBTN == PUSHED){
        return TCMODE;
    }
    if(PCBTN == PUSHED){
        return OFFICIALMODE;
    }
    if(MPBTN == PUSHED){
        return USERMODE;
    }
    return AVERAGEMODE;
}

// private function
void p_chkkey(void){
    /*
     キー押下優先度
     MAP最優先かつCP/PCと同時押し有効
     CP>PC>10key
     10keyは左上から優先される
     */

    //10key

    //3key(update)
    if(MPBTN == PUSHED){
        this->keyvalue = MP_PUSHED;
    }
    if(CPBTN == PUSHED){
        this->keyvalue += CP_PUSHED;
    }else if(PCBTN == PUSHED){
        this->keyvalue += PC_PUSHED;
    }
}
void p_bpout(void){

}
void p_outmain(void){
    switch (mainIng) {
    }
}
void p_outext1(void){

}
void p_outext2(void){

}
