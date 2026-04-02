;-------------------------------------------------------------------------------
; Title: Seven Segment Display
;-------------------------------------------------------------------------------
;Program Details:
; The purpose of this program is increment, decrement, or clear the value seen
; on a seven-segment display. The display should be able to show the hexadecimal
; values from 0h to Fh.
; Inputs: Inputs from a keypad going into PORTB.
; Outputs: PORTD to a Seven-Segment Display
; Date: April 2, 2026
; File Dependencies / Libraries: AssemblyConfig.inc, xc.inc
; Compiler: ASM v3.10
; Computer OS: Windows 11 Pro
; Author: Jason Knight-Han
; Versions:
;       V1.0: Original Program
;-------------------------------------------------------------------------------
;===========================================================
;  SECTION: Include Section
;===========================================================
#include "./AssemblyConfig.inc"
#include "./CallFunction.inc"
#include <xc.inc>

;===========================================================
;  SECTION: Constant Memory Locations
;===========================================================
    previousPressedButton EQU 0x020
    pressedButton EQU 0x021
    queryResult EQU 0x022
    decisionResult EQU 0x023
 
    innerLoop EQU 0x30
    outerLoop EQU 0x31
 
    portDLed EQU 0x40
 ;===========================================================
;  SECTION: Parameters
;===========================================================
    increment EQU 0x01
    decrement EQU 0x02
    clear EQU 0x03
    keepSame EQU 0x04
 
    innerLoopCount EQU 0x05
    outerLoopCount EQU 0x05
;===========================================================
;  SECTION: Program
;===========================================================
    PSECT absdata,abs,ovrld        ; Do not change

    ORG          0H                ;Reset vector
    GOTO        _Setup
    ORG          0020H           ; Begin assembly at 0020H
 
_Setup:
    CLRF	queryResult,0
    CLRF	pressedButton,0
    CLRF	previousPressedButton,0
    CLRF	portDLed,0
    CLRF	WREG,0
    
    BANKSEL	TRISD
    CLRF	TRISD ; Make PORTD an output.
    BANKSEL	LATD
    CLRF	LATD
    BANKSEL	PORTD
    CLRF	PORTD ; Ensure output is 0.
    BANKSEL	ANSELD
    CLRF	ANSELD ; Digital inputs (not needed).
    
    MOVLW	0b11111000
    BANKSEL	TRISB
    MOVWF	TRISB ; Make PORTB an input.
    BANKSEL	LATB
    CLRF	LATB ; Clear output latch.
    MOVLW	0b00000111
    MOVWF	LATB
    BANKSEL	ANSELB
    CLRF	ANSELB ; Select digital inputs.
    BANKSEL	WPUB
    MOVLW	0b11111000
    MOVWF	WPUB
    BANKSEL	PORTB
    CLRF	PORTB
    
    MOVLW 0xC0
    BANKSEL LATD
    MOVWF LATD
   
_Execute:
    MOVLW	0xFF    
    BANKSEL	PORTB

    BCF		PORTB, 0			
    BTFSS	PORTB, 3			
    MOVLW	0x01				
    BTFSS	PORTB, 4			
    MOVLW	0x02				
    BTFSS	PORTB, 7			
    MOVLW	0x03			
    BSF		PORTB, 0
    
    MOVWF	queryResult,0
    
    LFSR	1,pressedButton
    MOVFF	POSTINC1,previousPressedButton
    MOVFF	POSTDEC1,pressedButton
    CLRF	queryResult,0
    
    ; Check if no button was pressed
    MOVLW	0xFF
    SUBWF	pressedButton, 0
    BZ		_keepSame
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Check if clear
    CLRF	STATUS,0
    CLRF	WREG,0
    MOVF	pressedButton,0,0 ;0x21
    SUBLW	clear
    BZ		_clearLed ; Finished because it is a 3
    
    ; Check if incrementing or decrementing
    CLRF	STATUS,0
    MOVF	pressedButton,0,0
    SUBWF	previousPressedButton,0
    BZ		_same ; Can either be two 1s or two 2s
    GOTO	_different ;temp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
_same:
    MOVF	pressedButton,0,0
    SUBLW	0x01 ; if same and 1
    BZ		_increment
    MOVLW	decrement ; if same and 2
    MOVWF	decisionResult,0
    GOTO	_Here

_increment:
    MOVLW	increment
    MOVWF	decisionResult,0
    GOTO	_Here
    
_decrement:
    MOVLW	decrement
    MOVWF	decisionResult,0
    GOTO	_Here
    
_keepSame:
    MOVLW	keepSame
    MOVWF	decisionResult,0
    GOTO	_Here
    
_clearLed:
    MOVLW	clear
    MOVWF	decisionResult,0
    GOTO	_Here 
_different:
    MOVLW	increment
    SUBWF	pressedButton,0,0
    BZ		_increment
    CLRF	STATUS,0
    MOVLW	decrement
    SUBWF	pressedButton,0,0
    BZ		_decrement
    CLRF	STATUS,0
    GOTO	_Here
    
_Here:
    ResetWreg	decisionResult
    SUBLW	0x01
    BZ		_incrementPortD
    ResetWreg	decisionResult
    SUBLW	0x02
    BZ		_decrementPortD
    ResetWreg	decisionResult
    SUBLW	0x03
    BZ		_zeroPortD
    ResetWreg	decisionResult
    GOTO	_exit
_incrementPortD:
    BANKSEL	portDLed
    INCF	portDLed
    GOTO	_FinalHere
_decrementPortD:
    BANKSEL	portDLed
    DECF	portDLed
    GOTO	_FinalHere 
_zeroPortD:
    BANKSEL	portDLed
    MOVLW	0x00
    MOVWF	portDLed
    GOTO	_FinalHere
_FinalHere:
    MOVLW	0x0F
    ANDWF	portDLed,1,0
    CLRF	STATUS,0
    MOVLW	0x00
    XORWF	portDLed,0,0
    BZ		_0
    CLRF	STATUS,0
    MOVLW	0x01
    XORWF	portDLed,0,0
    BZ		_1
    CLRF	STATUS,0
    MOVLW	0x02
    XORWF	portDLed,0,0
    BZ		_2
    CLRF	STATUS,0
    MOVLW	0x03
    XORWF	portDLed,0,0
    BZ		_3
    CLRF	STATUS,0
    MOVLW	0x04
    XORWF	portDLed,0,0
    BZ		_4
    CLRF	STATUS,0
    MOVLW	0x05
    XORWF	portDLed,0,0
    BZ		_5
    CLRF	STATUS,0
    MOVLW	0x06
    XORWF	portDLed,0,0
    BZ		_6
    CLRF	STATUS,0
    MOVLW	0x07
    XORWF	portDLed,0,0
    BZ		_7
    CLRF	STATUS,0
    MOVLW	0x08
    XORWF	portDLed,0,0
    BZ		_8
    CLRF	STATUS,0
    MOVLW	0x09
    XORWF	portDLed,0,0
    BZ		_9
    CLRF	STATUS,0
    MOVLW	0x0A
    XORWF	portDLed,0,0
    BZ		_10
    CLRF	STATUS,0
    MOVLW	0x0B
    XORWF	portDLed,0,0
    BZ		_11
    CLRF	STATUS,0
    MOVLW	0x0C
    XORWF	portDLed,0,0
    BZ		_12
    CLRF	STATUS,0
    MOVLW	0x0D
    XORWF	portDLed,0,0
    BZ		_13
    CLRF	STATUS,0
    MOVLW	0x0E
    XORWF	portDLed,0,0
    BZ		_14
    CLRF	STATUS,0
    MOVLW	0x0F
    XORWF	portDLed,0,0
    BZ		_15

_0:
    MOVLW 0xC0
    BANKSEL LATD
    MOVWF LATD
    GOTO _exit
_1:
    BANKSEL LATD
    MOVLW 0xF9
    MOVWF LATD
    GOTO _exit
_2:
    BANKSEL LATD
    MOVLW 0xA4
    MOVWF LATD
    GOTO _exit
_3:
    BANKSEL LATD
    MOVLW 0xB0
    MOVWF LATD
    GOTO _exit
_4:
    BANKSEL LATD
    MOVLW 0x99
    MOVWF LATD
    GOTO _exit
_5:
    BANKSEL LATD
    MOVLW 0x92
    MOVWF LATD
    GOTO _exit
_6:
    BANKSEL LATD
    MOVLW 0x82
    MOVWF LATD
    GOTO _exit
_7:
    BANKSEL LATD
    MOVLW 0xF8
    MOVWF LATD
    GOTO _exit
_8:
    BANKSEL LATD
    MOVLW 0x80
    MOVWF LATD
    GOTO _exit
_9:
    BANKSEL LATD
    MOVLW 0x90
    MOVWF LATD
    GOTO _exit
_10:
    BANKSEL LATD
    MOVLW 0x88
    MOVWF LATD
    GOTO _exit
_11:
    BANKSEL LATD
    MOVLW 0x83
    MOVWF LATD
    GOTO _exit
_12:
    BANKSEL LATD
    MOVLW 0xC6
    MOVWF LATD
    GOTO _exit
_13:
    BANKSEL LATD
    MOVLW 0xA1
    MOVWF LATD
    GOTO _exit
_14:
    BANKSEL LATD
    MOVLW 0x86
    MOVWF LATD
    GOTO _exit
_15:
    BANKSEL LATD
    MOVLW 0x8E
    MOVWF LATD
    GOTO _exit
_exit:
    CALL	Delay
    GOTO	_Execute
    END