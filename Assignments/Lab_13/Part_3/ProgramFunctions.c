#include <xc.h>
#include "ProgramVariables.h"

void ConfigurePowerOnLed(void)
{
    TRISDbits.TRISD3 = 0b0;
    LATDbits.LATD3 = 0b0;
    ANSELDbits.ANSELD3 = 0b0;
    WPUDbits.WPUD3 = 0b0;
    PowerOnLed = 1;
}

void ConfigureRefernce(void)
{
    PMD0bits.CLKRMD = 0b0;

    // Set internal reference
    OSCFRQbits.FRQ = 0b0010; // 4-MHz for HFINTOSC
    OSCTUNEbits.TUN = 0b000000; // Oscillator tune
    OSCENbits.HFOEN = 0b1; // Enable HFOEN
    
    // Wait till oscillator is ready
    while(!OSCSTATbits.HFOR);
    OSCCON1bits.NDIV = 0b0000; // divide-by-1
    OSCCON1bits.NOSC = 0b110; // HFINTOSC
    //OSCCON2bits.CDIV = 0b0000;
    
    // Send reference clock to clock pin
    CLKRCONbits.DIV = 0b000;
    CLKRCONbits.DC = 0b10;
    CLKRCLKbits.CLK = 0b0001;
    CLKRCONbits.EN = 0b1;
}

void TMR2_Initialize(void)
{
    TRISBbits.TRISB0 = 0b0;
    ANSELBbits.ANSELB0 = 0b0;
    
    // Timer2 Clock Source - FOSC/4 == 1MHz
    T2CLKbits.CS = 0x01;

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLTbits.PSYNC = 0b0;
    T2HLTbits.CKPOL = 0b0;
    T2HLTbits.CKSYNC = 0b0;
    T2HLTbits.MODE = 0b0;

    // T2RSEL T2CKIPPS pin; 
    T2RSTbits.RSEL = 0x00;

    // PR2 155 - period register
    T2PRbits.PR2 = 0x9B;

    // TMR2 0 - counter register
    T2TMRbits.TMR2 = 0x00;

    // Clearing IF flag.
    PIR4bits.TMR2IF = 0;

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CONbits.OUTPS = 0b0000;
    T2CONbits.CKPS = 0b111; // 128
    T2CONbits.ON = 0b1;
}

void TMR2_StartTimer(void)
{
    T2CONbits.TMR2ON = 1;
}

void PWM_Output_B1_Enable (void){
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    // Set B1 as the output of CCP2
    TRISBbits.TRISB1 = 0b0;
    ANSELBbits.ANSELB1 = 0b0;
    RB1PPS = 0x0A;

    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
}

void PWM2_Initialize(void)
{
    CCP2CONbits.MODE = 0b1100; // PWM Mode
    CCP2CONbits.FMT = 0b0; // Right Aligned FMT
    CCP2CONbits.CCP2EN = 0b1; // Enabled CCP module
	
	// RH 127; 
	CCPR2H = 0x03;    
	
	// RL 192; 
	CCPR2L = 0xFF;    

	// Selecting Timer 2
	CCPTMRS0bits.C2TSEL = 0x1;
}

void PWM2_LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP2CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue & 0xFF;
    }
    else
    {
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue & 0xFF;
    }
}

void InitializeServoControl(void)
{
    TRISAbits.TRISA0 = 1; // Enable PORTA0 as a input - Right
    WPUAbits.WPUA0 = 1;
    ANSELAbits.ANSELA0 = 0;
    
    TRISAbits.TRISA1 = 1; // Enable PORTA1 as a input - Left
    WPUAbits.WPUA1 = 1;
    ANSELAbits.ANSELA1 = 0;

    //PIE0bits.IOCIE = 1; // Enable IOC interrupts
    
    IOCAPbits.IOCAP0 = 0;
    IOCANbits.IOCAN0 = 1; // Negative edge change detection
    IOCAFbits.IOCAF0 = 0; // Clearing flag
    
    IOCAPbits.IOCAP1 = 0;
    IOCANbits.IOCAN1 = 1; // Negative edge change detection
    IOCAFbits.IOCAF1 = 0; // Clearing flag
}