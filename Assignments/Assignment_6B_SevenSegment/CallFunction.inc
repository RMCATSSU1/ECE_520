PSECT absdata,abs,ovrld        ; Do not change

ORG 0x300

Delay:
	MOVLW outerLoopCount
	MOVWF outerLoop,0
	MOVLW innerLoopCount
	MOVWF innerLoop,0
    _AGAIN:    
	DECF innerLoop,1,0
	NOP
	NOP
	NOP
	NOP
	NOP
	BNZ _AGAIN
	MOVWF innerLoop,0
	DECF outerLoop,1,0
	BNZ _AGAIN
	RETURN

ResetWreg MACRO fileReg
    CLRF	STATUS,0
    CLRF	WREG,0
    MOVF	fileReg,0
ENDM