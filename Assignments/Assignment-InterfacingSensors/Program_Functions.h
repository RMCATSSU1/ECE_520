#ifndef PROGRAM_FUNCTIONS_H
#define PROGRAM_FUNCTIONS_H

#include <xc.h>
#include "Init_Variables.h"

/*******************************************************************************
 * PROJECT:     Password Entry and Notification System
 * FILENAME:    Program_Functions.h
 * AUTHOR:      Jason Knight-Han
 * DATE:        April 27, 2026
 * VERSION:     1.0.0
 * * DESCRIPTION:
 * This header file contains the function prototypes and Interrupt Service 
 * Routine (ISR) declarations for the system. It defines the interface for 
 * hardware initialization, timer management, and the password validation 
 * state machine.
 * * INTERRUPT ARCHITECTURE:
 * - INT0 (High Priority): Buzzer/Input Trigger
 * - INT1 (Low Priority): Counter Input
 * - TMR0/TMR1: System timing and debouncing
 ******************************************************************************/

void __interrupt(irq(IRQ_INT0), high_priority) INT0_ISR(void);
void __interrupt(irq(IRQ_INT1), low_priority) INT1_ISR(void);
void __interrupt(irq(IRQ_TMR0), __high_priority) TMR0_ISR(void);
void __interrupt(irq(IRQ_TMR1), __high_priority) TMR1_ISR(void);

void BuzzerInterruptConfig (void);
void CounterInterruptConfig (void);

void ExtInterruptEnable (void);
void TimerEnable (void);
void PowerOnLed (void);
void TurnOnBuzzer(void);
void IncorrectPassword (void);
void CorrectPassword (void);
void Count (void);

#endif