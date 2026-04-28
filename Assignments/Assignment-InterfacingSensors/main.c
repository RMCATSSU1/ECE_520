/*******************************************************************************
 * PROJECT:     Password Entry and Notification System
 * FILENAME:    main.c
 * AUTHOR:      Jason Knight-Han
 * DATE:        April 27, 2026
 * VERSION:     1.0.0
 * * TARGET MCU:  PIC Microcontroller (XC8 Compiler)
 * HARDWARE:    Custom PCB / Development Board
 * COMPUTER:    Window
 * IDE/TOOL:    MPLAB X IDE
 * * DESCRIPTION:
 * This program implements a security-based counter system. It initializes 
 * external interrupts on PORTC for a buzzer and a counter. The main loop 
 * monitors a count flag; if triggered, it validates a specific password 
 * sequence (Lower: 3, Upper: 2). If matched, it triggers a success routine; 
 * otherwise, it indicates an incorrect entry.
 * * REVISION HISTORY:
 * 1.0.0 - Initial release. Configured INT0/INT1 and main logic loop.
 ******************************************************************************/
#include <xc.h> 
#include "Program_Config.h"
#include "Init_Variables.h"
#include "Program_Functions.h"
///////////////////////////////////////////////////////////////////////////////
void main(void)
{
    PowerOnLed(); // Requirement to have a power LED.
    BuzzerInterruptConfig(); // Configures PORTC0 for INT0 Interrupt.
    CounterInterruptConfig(); // Configures PORTC1 for INT1 Interrupt.
    TimerEnable();
    ExtInterruptEnable();
    
    while(1)
    {

        if(nowCount)
        {
            Count();
            
            if (lowerCount == 3 && upperCount == 2)
            {
                CorrectPassword();
                
                nowCount = upperCount = lowerCount = 0;
            }
            else
            {
                IncorrectPassword();
                nowCount = upperCount = lowerCount = 0;
            }
        }
    }
}