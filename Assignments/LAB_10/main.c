/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply turn on a relay when light.
 * Inputs: RD0
 * Outputs: RD1 (output)
 * Setup: C- Simulator
 * Date: April 9, 2026
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * PIC18F47K42.H from local drive
 * Compiler: XC8, V3.10
 * Author: Jason Knight-Han
 * Versions:
 *      V1.0: Original
*/

#include <xc.h> // must have this
#include "thisProgramHeader.h" // must have this
#include "C:\Program Files\Microchip\MPLABX\v6.30\packs\Microchip\PIC18F-K_DFP\1.15.303\xc8\pic\include\proc\pic18f47k42.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

void InitializePortD(void);
void TurnOnMachine(int isOn);
int GetRoomState(void);

void main(void) {
    int isOn = 0;
    
    InitializePortD();
    
    while(1)
    {
        isOn = GetRoomState();
        TurnOnMachine(isOn);
    }
}

void InitializePortD(void)
{
    TRISD = 0b11111101;
    ANSELD = 0b00000000;
    PORTD = 0b00000000;
    LATD = 0b00000000;
    WPUD = 0b00000000;
}

void TurnOnMachine(int isOn){
    if (isOn)
    {
        LATDbits.LATD1 = 0x01;
    }
    else
    {
        LATDbits.LATD1 = 0x00;
    }
}

int GetRoomState(void)
{
    int state = 0;
    
    if (PORTDbits.RD0 == 0){
        state = 1;
    }
    else
    {
        state = 0;
    }
    
    return state;
}