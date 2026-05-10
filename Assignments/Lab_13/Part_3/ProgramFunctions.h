#ifndef PROGRAMFUNCTIONS_H
#define PROGRAMFUNCTIONS_H

void ConfigurePowerOnLed(void);

void ConfigureRefernce(void);

void TMR2_Initialize(void);

void TMR2_StartTimer(void);

void PWM_Output_B1_Enable (void);

void PWM2_Initialize(void);

void PWM2_LoadDutyValue(uint16_t dutyValue);

void InitializeServoControl(void);
#endif