/*******************************************************************************
 * PROJECT:     Password Entry and Notification System
 * FILENAME:    Init_Variables.c
 * AUTHOR:      Jason Knight-Han
 * DATE:        April 27, 2026
 * VERSION:     1.0.0
 * 
 * DESCRIPTION:
 * This file contains global variable declarations and initialization routines
 * for the ADC, LCD, and IOC subsystems. It manages voltage measurement, 
 * digital conversion, and interrupt-driven input handling.
 *
 * HARDWARE SETUP:
 * - ADC Input: AN17 (RC7 IOC pin for falling edge trigger)
 * - Voltage Reference: 3.3V
 * - LED Indicator: RB5 (status/debug output)
 * - LCD Interface: 8-bit mode on PORTD with control on RA6/RA7
 ******************************************************************************/

#include "ProgramConfig.h"
#include "ProgramFunctions.h"
#include "ProgramVariables.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Vref 5.00 // voltage reference 
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];

void __interrupt(irq(IRQ_IOC), high_priority) IOCC_ISR(void);

void main(void)
{
    // IOC Pin Configuration
    TRISCbits.TRISC7 = 1;
    LATCbits.LATC7 = 0;
    ANSELCbits.ANSELC7 = 0;
    WPUCbits.WPUC7 = 1;

    // IOC Configuration
    IOCCPbits.IOCCP7 = 0;
    IOCCNbits.IOCCN7 = 1;
    IOCCFbits.IOCCF7 = 0;
    IPR0bits.IOCIP = 1;         // ? Add: High priority
    PIR0bits.IOCIF = 0;         // ? Add: Clear flag
    PIE0bits.IOCIE = 1;
    
    INTCON0bits.IPEN = 1;       // Enable interrupt priority
    INTCON0bits.GIEH = 1;       // Enable high priority interrupts
    INTCON0bits.GIEL = 1;       // Enable low priority interrupts

    LCD_Init();
    LCD_String_xy(1, 0, "LCD Init");

    // LED Pin
    TRISBbits.TRISB5 = 0;
    ANSELBbits.ANSELB5 = 0;
    WPUBbits.WPUB5 = 0;
    
    LATBbits.LATB5 = 1;
    __delay_ms(500);
    LATBbits.LATB5 = 0;
    ADC_Init();
    LATBbits.LATB5 = 1;
    __delay_ms(500);
    LATBbits.LATB5 = 0;
    __delay_ms(500);

    while (1) {        
        ADCON0bits.GO = 1; //Start conversion
        while (ADCON0bits.GO); //Wait for conversion done
        digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
        voltage= digital*((float)Vref/(float)(4095)); 
        
        /*It is used to convert integer value to ASCII string*/
        sprintf(data,"%.3f",voltage);
        strcat(data," V");	/*Concatenate result and unit to print*/
        LCD_Command(0x01);
        LCD_String_xy(1, 0, "Input Voltage:  ");
        LCD_String_xy(2,0,data);/*Send string data for printing*/
        
        CLRWDT();
        SLEEP();
    }
}

void __interrupt(irq(IRQ_IOC), high_priority) IOCC_ISR(void){
    if (IOCCFbits.IOCCF7){
        
        for (int i = 0; i < 100; i++){
            LATBbits.LATB5 = 1;
            __delay_ms(100);
            LATBbits.LATB5 = 0;
            __delay_ms(100);
        }
        
        IOCCFbits.IOCCF7 = 0;
    }
}