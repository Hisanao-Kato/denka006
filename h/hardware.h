#ifndef _HARDWARE_H
#define _HARDWARE_H
/*
@filename:hardware.h
@description:
*/


#include <stdlib.h>
#include "p24FJ64GA004.h"
/*
 @define
 */
// Pin assign 多分これは中で
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

// wakeupmode
#define AVERAGEMODE         0
#define TCMODE              1
#define OFFICIALMODE        2
#define VERSIONCHECKMODE    3

// Beep type
// TODO: 値を探してくる。
#define     BEEP_LONG
#define     BEEP_ACK
#define     BEEP_NACK

// Key
#define KEY_0   0
#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define KEY_7   7
#define KEY_8   8
#define KEY_9   9
#define KEY_F   10
#define KEY_P   11
#define KEY_N   12
#define KEY_M   13
#define KEY_D   14
#define KEY_E   15

#define KEY_CP  16
#define KEY_PC  17
#define KEY_MP  18
#define KEY_CP_MP   19
#define KEY_PC_MP   20

// HardWareObject
typedef struct {
    unsigned char keyvalue;
    unsigned char mainIndicator[2];
    unsigned char mainFirst[6];
    unsigned char mainSecond[6];
    unsigned char mainThird[6];
    unsigned char extFirst[6];
    unsigned char extSecond[6];
    unsigned char extThird[6];
    //ext2は考える

}HardwareObject;

// Function define
HardwareObject initHardware(void);
char freeHardware(HardwareObject obj);
char getWakeupMode(void);
char setBeep(char beeptype);













// Public value
struct OutputValue{
  long* nowtime;
  long* starttime;
  double* average;
  long* pcdist;
  //
}

// Public Function
/*
@function
@name:getWakeupMode
@return:char wakeupmode
@param:void
@description:

*/

/*
@function
@name:hwtimertask
@return:void
@param:void
@description:

*/

/*
@function
@name:setBeep
@return:char result
@param:char beeptype
@description:
*/

/*
@function
@name:setOutputValue
@return:char(result)
@param:char witch, char valuetype
@description:

*/

/*
@function
@name:getKeyValue
@return:char keyvalue
@param:void
@description:
*/

/*
@function
@name:getBackSensor
@return:char backsensorvalue
@param:void
@description:

*/

/*
@function
@name:getPulseCount
@return:char pulsecount
@param:void
@description:

*/


#endif
