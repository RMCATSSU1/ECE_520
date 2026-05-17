 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

/*
    Main application
*/
#include <stdio.h>
#include <time.h>

int random_number (int min_num, int max_num)
{
    int result = 0;
    int low_num = 0;
    int hi_num = 0;
    
    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1;
    }
    else
    {
        low_num = max_num +1;
        hi_num = low_num;
    }
    
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

int main(void)
{
    SYSTEM_Initialize();
    UART2_Initialize();
    //UART2_TransmitDisable();
    UART2_ReceiveEnable();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 


    while(1)
    {
        printf("Please enter 'A' to turn on and 'B' to turn off:\r\n");
        __delay_ms(3000);
        
        if (UART2_IsRxReady())
        {
            uint8_t reading = UART2_Read();
            printf("%d\r\n", reading);
            
            if (reading == 'A')
            {
            PORTBbits.RB1 = 1;
            }
            if (reading == 'B')
            {
            PORTBbits.RB1 = 0;
            }
        }
    }    
}