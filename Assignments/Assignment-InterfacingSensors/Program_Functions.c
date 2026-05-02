
#include "Program_Config.h"
#include "Init_Variables.h"
#include "Program_Functions.h"
#include <stdio.h>

void BuzzerInterruptConfig (void)
{
    // Buzzer output.
    TRISEbits.TRISE1 = 0b0;
    PORTEbits.RE1 = 0b0;
    ANSELEbits.ANSELE1 = 0b0;
    WPUEbits.WPUE1 = 0b0;
    
    // RB0 Physical Configuration (Pin).
    TRISBbits.TRISB0 = 0b1;
    PORTBbits.RB0 = 0b0;
    LATBbits.LATB0 = 0b0;
    ANSELBbits.ANSELB0 = 0b0;
    WPUBbits.WPUB0 = 0b1;
    
    // Buzzer Interrupt
    INTCON0bits.INT0EDG = 0;    // Interrupt on falling edge of INT0 pin
    IPR1bits.INT0IP = 1;        // High priority
    PIR1bits.INT0IF = 0;        // Clear interrupt flag
    PIE1bits.INT0IE = 1;        // INT0 enable.
}
void CounterInterruptConfig (void)
{
     // RB1 Physical Configuration (Pin).
    TRISBbits.TRISB1 = 0b1;
    PORTBbits.RB1 = 0b0;
    LATBbits.LATB1 = 0b0;
    ANSELBbits.ANSELB1 = 0b0;
    WPUBbits.WPUB1 = 0b0;
    
    // Number Interrupt
    INT1PPS = 0b001001;         // B1
    INTCON0bits.INT1EDG = 0;    // Interrupt on falling edge of INT1 pin
    IPR5bits.INT1IP = 0;        // Low priority
    PIR5bits.INT1IF = 0;        // Clear interrupt flag
    PIE5bits.INT1IE = 1;        // INT1 enable.
    
}
void ExtInterruptEnable (void)
{
    INTCON0bits.IPEN = 1;       // Enable interrupt priority
    INTCON0bits.GIEH = 1;       // Enable high priority interrupts
    INTCON0bits.GIEL = 1;       // Enable low priority interrupts
    
    // Change IVTBASE if required
    IVTBASEU = 0x00; // Optional
    IVTBASEH = 0x40; // Default is 0x0008
    IVTBASEL = 0x08; 
}
void TimerEnable (void)
{
    // Enabling timer counter ports as inputs.
    PORTAbits.RA0 = 0b0;
    PORTAbits.RA1 = 0b0;

    TRISAbits.TRISA0 = 0b1;
    TRISAbits.TRISA1 = 0b1;
    
    LATAbits.LATA0 = 0b0;
    LATAbits.LATA1 = 0b0;

    ANSELAbits.ANSELA0 = 0b0;
    ANSELAbits.ANSELA1 = 0b0;

    WPUAbits.WPUA0 = 0b0;
    WPUAbits.WPUA1 = 0b0;
    
    // PPS
    T0CKIPPS = 0b000000; // PORTA.A0
    T1CKIPPS = 0b000001; // PORTA.A1
    T0CON1 = 0b00110000; // T0CLKIPPS - Inverted. Not sync. No pre-scale.
    T1CLK = 0b00000000;  // T1CLKIPPS
    
    IPR3bits.TMR0IP = 1; // High priority
    PIE3bits.TMR0IE = 0; // Disable timer 0 interrupt
    PIR3bits.TMR0IF = 0; // Clear flag
    
    IPR4bits.TMR1IP = 1; // High priority
    PIE4bits.TMR1IE = 0; // Disable timer 1 interrupt
    PIR4bits.TMR1IF = 0; // Clear flag
    
    // Turn off counting
    T0CON0bits.EN = 0;
    T1CON = 0b00000000;  // Bit 0 = 0 (Turn OFF Timer1)

    // Counter Registers.
    TMR0H = 255; 
    TMR0L = 0; 
    TMR1H = 255; 
    TMR1L = 0; 
    
    PIR3bits.TMR0IF = 0; // Clear flag
    PIR4bits.TMR1IF = 0; // Clear flag
}
void __interrupt(irq(IRQ_INT0), high_priority) INT0_ISR(void)
{
    TurnOnBuzzer(); // Play SOS tone.
    INT0IF = 0; // Clear flag 
}
void __interrupt(irq(IRQ_INT1), low_priority) INT1_ISR(void)
{
    nowCount = 1; // Set counting flag. 
    INT1IF = 0; // Clear flag 
}
void __interrupt(irq(IRQ_TMR0), __high_priority) TMR0_ISR(void)
{
    TMR0L = 0; 
    TMR0IF = 0;
}
void __interrupt(irq(IRQ_TMR1), __high_priority) TMR1_ISR(void)
{
    TMR1L = 0; 
    TMR1IF = 0;
}
void PowerOnLed (void)
{
    // Initialization for Peripheral Control.
    PORTEbits.RE0 = 0;
    ANSELEbits.ANSELE0 = 0b0; // Digital inputs.
    TRISEbits.TRISE0 = 0b0; // PORTD as an OUTPUT.
    LATEbits.LATE0 = 0b0; // Clear latches
    
    ANSELBbits.ANSELB2 = 0b0; // Digital inputs.
    TRISBbits.TRISB2 = 0b0; // PORTB2 as an OUTPUT.
    LATBbits.LATB2 = 0b0; // Clear latches
    
    ANSELBbits.ANSELB3 = 0b0; // Digital inputs.
    TRISBbits.TRISB3 = 0b0; // PORTB3 as an OUTPUT.
    LATBbits.LATB3 = 0b0; // Clear latches
    
    ANSELBbits.ANSELB4 = 0b0; // Digital inputs.
    TRISBbits.TRISB4 = 0b0; // PORTB4 as an OUTPUT.
    LATBbits.LATB4 = 0b0; // Clear latches
    
    PowerOnLed_E0 = ON;
    CountingRed_B2 = OFF;
    CountingGreen_B3 = OFF;
    MotorPin_B4 = OFF;
}
void TurnOnBuzzer(void)
{
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    
    Buzzer_E1 = ON;
    __delay_ms(500);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(500);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(500);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
    Buzzer_E1 = ON;
    __delay_ms(200);
    Buzzer_E1 = OFF;
    __delay_ms(200);
}
void IncorrectPassword(void)
{
    Buzzer_E1 = ON;
    __delay_ms(500);
    Buzzer_E1 = OFF;
}
void CorrectPassword (void) // needs to control motor.
{
    MotorPin_B4 = ON;
    __delay_ms(3000);
    MotorPin_B4 = OFF;
}
void Count (void)
{
    CountingRed_B2 = ON;
    CountingGreen_B3 = OFF;
    __delay_ms(2000);
    CountingGreen_B3 = ON;
    CountingRed_B2 = OFF;
    /////////////////////////////////////////////////
    T0CON0bits.EN = 0; // Disable timer 0
    TMR0L = 0;
    T0CON0bits.EN = 1; // Enable timer 0
    __delay_ms(2000);
    T0CON0bits.EN = 0; // Disable timer 0
    lowerCount = TMR0L;
    TMR0L = 0;
    /////////////////////////////////////////////////
    CountingGreen_B3 = OFF;
    CountingRed_B2 = ON;
    /////////////////////////////////////////////////
    __delay_ms(2000);
    /////////////////////////////////////////////////
    CountingRed_B2 = OFF;
    CountingGreen_B3 = ON;
    /////////////////////////////////////////////////
    T1CON = 0b00000000;  // Disable timer 1
    TMR1L = 0;
    T1CON = 0b00000001;  // Enable timer 1
    __delay_ms(2000);
    T1CON = 0b00000000;  // Disable timer 1
    upperCount = TMR1L;
    TMR1L = 0;
    /////////////////////////////////////////////////
    CountingRed_B2 = OFF;
    CountingGreen_B3 = OFF;
    
    char buffer[16];
    sprintf(buffer, "%d", lowerCount);
    LCD_String_xy(2, 9, buffer);
    sprintf(buffer, "%d", upperCount);
    LCD_String_xy(2, 8, buffer);

}
void LCD_Init()
{
    // Initialize LATs before enabling outputs
    LATD = 0x00;             // Clear data port
    LATAbits.LATA7 = 0;     // RS low
    LATAbits.LATA6 = 0;     // EN low

    TRISD = 0x00;            // PORTD all output (LCD data)
    TRISAbits.TRISA7 = 0;   // RA7 output (RS)
    TRISAbits.TRISA6 = 0;   // RA6 output (EN)

    ANSELD = 0x00;           // PORTD all digital
    ANSELAbits.ANSELA7 = 0; // RA7 digital
    ANSELAbits.ANSELA6 = 0; // RA6 digital

    __delay_ms(250);         // Power-on delay
    LCD_Command(0x38);       // 8-bit, 2-line, 5x7 font
    LCD_Command(0x01);       // Clear display
    LCD_Command(0x0C);       // Display on, cursor off
    LCD_Command(0x06);       // Increment cursor
}

void LCD_Command(char cmd)
{
    ldata = cmd;
    RS = 0;
    _EN = 1;
    NOP();
    _EN = 0;
    __delay_ms(3);
}

void LCD_Char(char dat)
{
    ldata = dat;
    RS = 1;
    _EN = 1;
    NOP();
    _EN = 0;
    __delay_ms(1);
}

void LCD_String(const char *msg)
{
    while(*msg != 0)
    {
        LCD_Char(*msg);
        msg++;
    }
}

void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = 0;
    if(row == 1)
        location = 0x80 | (pos & 0x0F);
    else
        location = 0xC0 | (pos & 0x0F);
    LCD_Command(location);
    LCD_String(msg);
}