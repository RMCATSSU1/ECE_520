#ifndef PROGRAMCONFIGURATION_H
#define PROGRAMCONFIGURATION_H

// Configuration Word 1L
#pragma config FEXTOSC = HS
#pragma config RSTOSC = HFINTOSC_1MHZ

// Configuration Word 1H
#pragma config CLKOUTEN = ON
#pragma config PR1WAY = ON
#pragma config CSWEN = ON
#pragma config FCMEN = ON

// Configuration Word 2L
#pragma config MCLRE = INTMCLR 
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON
#pragma config IVT1WAY = ON
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS

// Configuration Word 2H
#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF

// Configuration Word 3L
#pragma config WDTCPS = WDTCPS_14
#pragma config WDTE = OFF

// Configuration Word 3H
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC

// Configuration Word 4L
#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF

// Configuration Word 4H
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config WRTSAF = OFF
#pragma config LVP = ON

// Configuration Word 5L
#pragma config CP = OFF

#define _XTAL_FREQ 4000000  // Must match RSTOSC config

#define FCY    _XTAL_FREQ/4
#endif