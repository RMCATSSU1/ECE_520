#ifndef INIT_VARIABLES_H
#define INIT_VARIABLES_H
#include <xc.h>

/*******************************************************************************
 * PROJECT:     Password Entry and Notification System
 * FILENAME:    Init_Variables.h
 * AUTHOR:      Jason Knight-Han
 * DATE:        April 27, 2026
 * VERSION:     1.0.0
 * * DESCRIPTION:
 * Global variable declarations and hardware I/O mapping. This file defines 
 * the pin aliases for LEDs, Buzzers, and Sensors, and initializes the 
 * volatile flags used for interrupt-driven counting.
 * * HARDWARE CONNECTIONS:
 * - RE0: Power Status LED
 * - RE1: System Buzzer
 * - RB2: Red Status LED (Counting)
 * - RB3: Green Status LED (Counting)
 * - RB4: Motor/Actuator Control
 ******************************************************************************/

// On/Off
#define ON 1u 
#define OFF 0u

// GPIO Set-Up
#define Buzzer_E1  LATEbits.LATE1  
#define PowerOnLed_E0 LATEbits.LATE0
#define CountingRed_B2 LATBbits.LATB2
#define CountingGreen_B3 LATBbits.LATB3
#define MotorPin_B4 LATBbits.LATB4

// Photoresistor Counting Pins
#define LowerCountPin 0u
#define UpperCountPin 1u

// Seceret Code
const uint8_t SeceretCode = 0x23;

// Dynamic Variables/Flags
volatile uint8_t  upperCount = 0;
volatile uint8_t  lowerCount = 0;
volatile uint8_t  nowCount = 0;

#endif