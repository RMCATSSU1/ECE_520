#include "ProgramConfig.h"
#include "ProgramFunctions.h"
#include "ProgramVariables.h"
#include <stdio.h>

void LCD_Init()
{
    // Initialize LCD PORTs
    // LCD Data Lines
    LATD = 0x00;             // Clear data port
    TRISD = 0x00;            // PORTD all output (LCD data)
    ANSELD = 0x00;           // PORTD all digital
    WPUD = 0x00;
    
    // LCD Control Lines
    LATAbits.LATA7 = 0;     // RS low
    LATAbits.LATA6 = 0;     // EN low
    TRISAbits.TRISA7 = 0;   // RA7 output (RS)
    TRISAbits.TRISA6 = 0;   // RA6 output (EN)
    ANSELAbits.ANSELA7 = 0; // RA7 digital
    ANSELAbits.ANSELA6 = 0; // RA6 digital
    WPUAbits.WPUA7 = 0;
    WPUAbits.WPUA6 = 0;

    __delay_ms(2500);         // Power-on delay
    LCD_Command(0x38);       // 8-bit, 2-line, 5x7 font
    LCD_Command(0x01);       // Clear display
    LCD_Command(0x0C);       // Display on, cursor off
    LCD_Command(0x06);       // Increment cursor
}

void LCD_Command(char cmd)
{
    Lcd_Latch = cmd;
    RS = 0;
    _EN = 1;
    NOP();
    _EN = 0;
    __delay_ms(3);
}

void LCD_Char(char dat)
{
    Lcd_Latch = dat;
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

void ADC_Init(void)
{
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSELA0 = 1;
    WPUAbits.WPUA0 = 0;
    
    // --- ADC configuration ---
    ADCON0bits.FM  = 1;                 // Right-justify result
    ADCON0bits.CS  = 1;                 // Use internal ADCRC clock
    ADPCH          = 0x00;              // Select RA0 as analog channel
    TRISAbits.TRISA0  = 1;             // RA0 as input
    ANSELAbits.ANSELA0 = 1;            // RA0 as analog
    ADCON0bits.ON  = 1;                 // Enable ADC
}