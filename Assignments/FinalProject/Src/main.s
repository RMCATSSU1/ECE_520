.syntax unified
.thumb
.cpu cortex-m4

@ ============================================
@ Base addresses from STM32L476 Reference Manual
@ ============================================
.equ RCC_BASE,      0x40021000
.equ RCC_AHB2ENR,   0x4C
.equ GPIOA_BASE,    0x48000000
.equ GPIOA_MODER,   0x00
.equ GPIOA_ODR,     0x14

@ ============================================
@ Bit masks and values
@ ============================================
.equ RCC_AHB2ENR_GPIOAEN, (1 << 0)   @ Bit 0: enable GPIOA clock
.equ GPIO_MODER5_MASK,    (3 << 10)  @ Bits 11:10 for PA5 mode
.equ GPIO_MODER5_OUT,     (1 << 10)  @ 01 = general-purpose output
.equ GPIO_ODR5,           (1 << 5)   @ Bit 5: PA5 output data

.equ DELAY_COUNT_LONG, 0x100000            @ Delay loop count
.equ DELAY_COUNT_SHORT, 0x10000            @ Delay loop count
.equ STD_DELAY, 0x200000
@ ============================================
@ Main function
@ ============================================
.global main
.type main, %function

main:
@ ----------------------------------------
@ Step 1: Enable clock for GPIOA
@ ----------------------------------------
@ Load RCC base address into r0
LDR r0, =RCC_BASE
@ Read current RCC_AHB2ENR register value
LDR   r1, [r0, #RCC_AHB2ENR]

@ Set bit 0 (GPIOAEN) to enable GPIOA clock
ORR   r1, r1, #RCC_AHB2ENR_GPIOAEN

@ Write back to RCC_AHB2ENR
STR   r1, [r0, #RCC_AHB2ENR]

@ ----------------------------------------
@ Step 2: Configure PA5 as output
@ ----------------------------------------
@ Load GPIOA base address into r0
LDR   r0, =GPIOA_BASE

@ Read current MODER register value
LDR   r1, [r0, #GPIOA_MODER]

@ Clear mode bits for PA5 (bits 11:10)
BIC   r1, r1, #GPIO_MODER5_MASK

@ Set PA5 mode to output (01)
ORR   r1, r1, #GPIO_MODER5_OUT

@ Write back to GPIOA_MODER
STR   r1, [r0, #GPIOA_MODER]

@ ----------------------------------------
@ Step 3: Main loop - toggle LED forever
@ ----------------------------------------

loop:
@ Read current ODR value
LDR r1, [r0, #GPIOA_ODR]
@ Toggle bit 5 (PA5) using XOR
EOR   r1, r1, #GPIO_ODR5

@ Write back to GPIOA_ODR (LED changes state)
STR   r1, [r0, #GPIOA_ODR]

@ ----------------------------------------
@ Step 4: Delay
@ ----------------------------------------
@ Load delay count into r2
LDR   r2, =STD_DELAY

delay_loop:
@ Decrement counter
SUBS r2, r2, #1
@ Branch back if not zero
BNE   delay_loop

@ Return to main loop (blink continues forever)
B     loop

@ We never reach here, but good practice:
.size main, .-main

