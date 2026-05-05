#ifndef PROGRAM_CONFIG_H
#define	PROGRAM_CONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.  
/*******************************************************************************
 * PROJECT:     Password Entry and Notification System
 * FILENAME:    Program_Config.h
 * AUTHOR:      Jason Knight-Han
 * DATE:        April 27, 2026
 * VERSION:     1.0.0
 * * DESCRIPTION:
 * This header file contains the Configuration Bits (Pragmas) for the MCU.
 * It defines the oscillator settings, watchdog timer behavior, and system 
 * protection levels. It also establishes the system clock frequency 
 * (_XTAL_FREQ) required for timing and delay functions.
 * * HARDWARE SETUP:
 * - Oscillator: External LP (32.768 kHz)
 * - Brown-out Reset: Enabled at 2.45V
 * - Programming: Low Voltage Programming (LVP) Enabled
 ******************************************************************************/
// CONFIG1L
#pragma config FEXTOSC = OFF        // Disable external oscillator
#pragma config RSTOSC = HFINTOSC_64MHZ  // Use internal oscillator at 64 MHz

// CONFIG1H
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = ON
#pragma config CSWEN = ON
#pragma config FCMEN = ON

// CONFIG2L
#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON
#pragma config IVT1WAY = ON
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS

// CONFIG2H
#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF

// CONFIG3L
#pragma config WDTCPS = WDTCPS_13
#pragma config WDTE = ON

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = LFINTOSC   

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF

// CONFIG4H
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config WRTSAF = OFF
#pragma config LVP = ON

// CONFIG5L
#pragma config CP = OFF

#define _XTAL_FREQ 64000000                // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#endif