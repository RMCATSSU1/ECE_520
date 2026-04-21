/*
 * File:    main.c
 * Device:  PIC18F47K42
 *
 * Description:
 * This program demonstrates the use of the INT0 external interrupt on a 
 * PIC18F47K42 using vectored interrupts (MVECEN = ON). The INT0 interrupt 
 * is mapped to pin RB0 via Peripheral Pin Select (PPS).
 *
 * Operation:
 * - LED2 (RD1) continuously toggles in the main loop.
 * - When a rising edge is detected on RB0 (INT0), an interrupt is triggered.
 * - The interrupt service routine (ISR) sets a flag (int0Flag).
 * - The main loop detects this flag and:
 *      1. Turns off LED2
 *      2. Blinks LED1 (RD0) eight times
 *      3. Returns to normal operation
 *
 * Key Features:
 * - Uses high-priority vectored interrupt (IRQ_INT0)
 * - Interrupt input mapped using PPS (INT0PPS = RB0)
 * - Minimal ISR design (flag-based handling)
 * - Demonstrates separation of ISR and main application logic
 *
 * Notes:
 * - RB0 must be configured as a digital input.
 *
 * Author:  Jason Knight-Han
 * Date:    04/21/26
 */
// CONFIG1L
#pragma config FEXTOSC = LP
#pragma config RSTOSC = EXTOSC

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
#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC

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

#include <xc.h>

#define LED1 LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define _ON  1
#define _OFF 0

#define _XTAL_FREQ 4000000
#define FCY (_XTAL_FREQ/4)

void __interrupt(irq(IRQ_INT0), __high_priority) INT0_ISR(void);

void InitializeLedPorts(void);
void InitializeInterruptPort(void);
void BlinkLedTwo(void);
void BlinkLedOne(void);

volatile int int0Flag = 0;

void main(void)
{
    InitializeLedPorts();
    InitializeInterruptPort();

    while (1)
    {
        BlinkLedTwo();

        if (int0Flag == 1)
        {
            int0Flag = 0;
            LED2 = _OFF;
            BlinkLedOne();
        }
    }
}

void InitializeLedPorts(void)
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;

    ANSELDbits.ANSELD0 = 0;
    ANSELDbits.ANSELD1 = 0;

    LATDbits.LATD0 = _OFF;
    LATDbits.LATD1 = _OFF;
}

void InitializeInterruptPort(void)
{
    TRISBbits.TRISB0 = 1;      // RB0 input
    ANSELBbits.ANSELB0 = 0;    // RB0 digital

    // Optional pull-up if your switch/input needs it:
    // WPUBbits.WPUB0 = 1;

    INT0PPS = 0x08;            // RB0 -> INT0

    INTCON0bits.INT0EDG = 1;   // interrupt on rising edge

    PIR1bits.INT0IF = 0;       // clear flag first
    IPR1bits.INT0IP = 1;       // high priority
    PIE1bits.INT0IE = 1;       // enable INT0

    INTCON0bits.IPEN = 1;      // enable priorities
    INTCON0bits.GIEH = 1;      // enable high-priority interrupts
    INTCON0bits.GIEL = 1;      // enable low-priority interrupts
}

void BlinkLedTwo(void)
{
    LED2 = _ON;
    LED2 = _OFF;
}

void BlinkLedOne(void)
{
    for (int i = 0; i < 8; i++)
    {
        LED1 = _ON;
        LED1 = _OFF;
    }
}

void __interrupt(irq(IRQ_INT0), __high_priority) INT0_ISR(void)
{
    int0Flag = 1;
    PIR1bits.INT0IF = 0;
}