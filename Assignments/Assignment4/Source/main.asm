//-----------------------------
// Title: ECE-520-Assignment 4
//-----------------------------
// Purpose: In this project we develop a heating and cooling control system. The 
// basic idea is that the user sets the desired temperature. If the environment
// temperature (e.g., room temp) is higher than the reference temperature, the
// cooling system will be turned on. On the other hand, if the environment
// temperature falls below the reference temperature, the heating systems will
// start. 
// Dependencies: Are you using any header files - if no say NONE
// Other: OS - Windows 11
// Compiler: pic-as V3.10
// Author: Jason Knight-Han
// OUTPUTS: PORTD1 to Blue LED, PORTD2 to RED LED 
// INPUTS: 
// Versions:
//  	V1.0: 03/12/26
//-----------------------------

;----------------
; PROGRAM INPUTS
;----------------
;The DEFINE directive is used to create macros or symbolic names for values.
;It is more flexible and can be used to define complex expressions or sequences of instructions.
;It is processed by the preprocessor before the assembly begins.
    
;---------------------
; Initialization
;---------------------
#include ".\AssemblyConfig.inc"
#include <xc.inc>
#define  measuredTempInput  0xFB ; this is the input value 48d
#define  refTempInput	    0x0F ; this is the input value 41d

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	  PORTD,1
 
;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
; It is simpler and is typically used for straightforward assignments.
;It directly substitutes the defined value into the code during the assembly process.
REG10   equ     0x10
REG11   equ     0x11
REG01   equ     0x01

RefTemp equ 0x20;
MeasuredTemp equ 0x21;
ContReg equ 0x22;

RefTempUpper equ 0x61
RefTempLower equ 0x62
 
MeasuredTempUpper equ 0x71
MeasuredTempLower equ 0x72
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    ORG 0x00                ;Reset vector

    ORG 0x20

    ;Initialize PORTD and set outputs to 0.
    MOVLW 0x00
    MOVWF STATUS
    MOVWF RefTempUpper
    MOVWF RefTempLower
    MOVWF ContReg
    MOVLW 0xFC 
    MOVWF TRISD,0
    
    ;Store reference and measured temperature
    MOVLW refTempInput
    MOVWF RefTemp,0
    MOVLW measuredTempInput
    MOVWF MeasuredTemp,0
    
    ;Convert reference temp to decimal and store in 0x60-0x62
    GOTO _ConvertRefTemp
_BACK1:
    
    MOVLW 0x00
    MOVWF STATUS,0
        ;Convert measured temp to decimal and store in 0x70-0x72
    GOTO _ConvertMeasuredTemp
_BACK2:
    
    ;Store reference and measured temperature
    MOVLW refTempInput
    MOVWF RefTemp,0
    MOVLW measuredTempInput
    MOVWF MeasuredTemp,0
    BTFSC MeasuredTemp,7,0
    GOTO CONVERT2SCOMPLEMENT2
_BACK3:
    GOTO COMPARE
    

_MAIN:
    MOVLW 0x0
    MOVFF ContReg,WREG
    MOVWF LATD,0
    SLEEP
    BRA _MAIN
    
;Convert refernce temp to decimal and store in 0x60-0x62
_ConvertRefTemp:
    _Divide10: 
	MOVLW 0x0A
	SUBWF RefTemp,1,0
	BNC _DIVIDE1
	INCF RefTempUpper,1,1
	GOTO _Divide10
    _DIVIDE1:
	BN GetRemainder1
	BRA _BACK1
    
GetRemainder1:
    COMF RefTemp,1,0
    INCF RefTemp,1,0
    MOVLW 0x0A
    SUBWF RefTemp,1,0
    COMF RefTemp,1,0
    INCF RefTemp,1,0
    MOVFF RefTemp,RefTempLower
    BRA _BACK1
    
;Convert measured temp to decimal and store in 0x70-0x72
_ConvertMeasuredTemp:
    BTFSC MeasuredTemp,7,0
    GOTO CONVERT2SCOMPLEMENT
_BACK:
    _DIVIDEBy10: 
	MOVLW 0x0A
	SUBWF MeasuredTemp,1,0
	BNC _DIVIDEBy1
	INCF MeasuredTempUpper,1,1
	GOTO _DIVIDEBy10
    _DIVIDEBy1:
	BN GetRemainder2
	BRA _BACK2
	
GetRemainder2:
    COMF MeasuredTemp,1,0
    INCF MeasuredTemp,1,0
    MOVLW 0x0A
    SUBWF MeasuredTemp,1,0
    COMF MeasuredTemp,1,0
    INCF MeasuredTemp,1,0
    MOVFF MeasuredTemp,MeasuredTempLower
    BRA _BACK2
    
    ORG 0x300
COMPARE:
    MOVF MeasuredTemp,0,0
    CPFSEQ RefTemp,0
    GOTO COMPAREGREATER
    MOVLW 0x00 ; To make ContReg 1 if measured is equal
    MOVWF ContReg,0
    GOTO _MAIN
COMPAREGREATER:; To make ContReg 1 if measured is less
    CPFSGT RefTemp,0
    GOTO MEASUREDGREATERTHANREFERENCE
    MOVLW 0x01
    MOVWF ContReg,0
    GOTO _MAIN
MEASUREDGREATERTHANREFERENCE:; To make ContReg 2 if measured is greater
    MOVLW 0x02
    MOVWF ContReg,0
    BSF LATD,1,0
    GOTO _MAIN

CONVERT2SCOMPLEMENT: ; To convert measured if negative
    COMF MeasuredTemp,1,0
    INCF MeasuredTemp,1,0
    GOTO _BACK
    
CONVERT2SCOMPLEMENT2: ; To convert measured if negative
    COMF MeasuredTemp,1,0
    INCF MeasuredTemp,1,0
    GOTO _BACK3
    END