#include <xc.h>
#include "ProgramConfiguration.h"
#include "ProgramFunctions.h"
#include "ProgramVariables.h"
uint16_t Current = 47;

void main(void) {
    ConfigurePowerOnLed();
    ConfigureRefernce();
 
    TMR2_Initialize();
    TMR2_StartTimer();        
    
    PWM_Output_B1_Enable();
    PWM2_Initialize();
    
    InitializeServoControl();
    
    PWM2_LoadDutyValue(Current); // initialize CCPR2H/L

    while(1)
    {
        if (PIR4bits.TMR2IF)
        {
            PIR4bits.TMR2IF = 0;
            myLED ^= 1; // ISR code goes here
        }
        
        if (IOCAFbits.IOCAF1) // Left
        {            
            if (Current >= MaxLeft)
            {
                Current = MaxLeft;
            }
            else
            {
                Current++;
                PWM2_LoadDutyValue(Current);
            }
            
            IOCAFbits.IOCAF1 = 0;
        }
        
        if (IOCAFbits.IOCAF0) // Right
        {
            if (Current <= MaxRight)
            {
                Current = MaxRight;
            }
            else
            {
                Current--;
                PWM2_LoadDutyValue(Current);
            }
            
            IOCAFbits.IOCAF0 = 0;
        }
    }
}