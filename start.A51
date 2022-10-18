;
; sudochen@163.com
; This OS Just for Studing
; Please contact the author for commercial use
; All Right Reserved
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY
; EXTRN function is called in this file but defined in others files
; PUBLIC means export this function for other call
; NAME defined the module name

		NAME	C51STARTUP		
		
		EXTRN CODE (?C_START)
		EXTRN CODE (task_switch)
		EXTRN CODE (uart_irq)
		PUBLIC	?C_STARTUP
		PUBLIC	SCHELDULE
		
		ORG		0000H				;absolute code at addr 0x00
?C_STARTUP:		
		LJMP	START	
		ORG		000BH				;absolute code at addr 0x0B timer_0_irq
		SETB	F0
		LJMP	SCHELDULE
		ORG		0023H
		LJMP	UART_SYS

START:
		MOV		R0,		#0FFH
CLB:	MOV		@R0,	#00H
		DJNZ	R0,		CLB
		MOV		SP,		#0D0H
		LJMP	?C_START
		
UART_SYS:
		; all we save 13 system registers
		CLR		EA
		PUSH 	ACC
		PUSH	B
		PUSH	PSW
		PUSH	DPL
		PUSH	DPH
		USING	0
		PUSH	AR0
		PUSH	AR1
		PUSH	AR2
		PUSH	AR3
		PUSH	AR4
		PUSH	AR5
		PUSH	AR6
		PUSH	AR7
		CALL	uart_irq
		; we restore 13 system	registers
		POP		AR7
		POP		AR6
		POP		AR5
		POP		AR4
		POP		AR3
		POP		AR2
		POP		AR1
		POP		AR0
		POP		DPH
		POP		DPL
		POP		PSW
		POP		B
		POP		ACC
		SETB	EA
		RETI

SCHELDULE:
		; all we save 13 system registers
		CLR		EA
		PUSH 	ACC
		PUSH	B
		PUSH	PSW
		PUSH	DPL
		PUSH	DPH
		USING	0
		PUSH	AR0
		PUSH	AR1
		PUSH	AR2
		PUSH	AR3
		PUSH	AR4
		PUSH	AR5
		PUSH	AR6
		PUSH	AR7
		CALL	task_switch
		MOV		SP,	R7
		; we restore 13 system	registers
		POP		AR7
		POP		AR6
		POP		AR5
		POP		AR4
		POP		AR3
		POP		AR2
		POP		AR1
		POP		AR0
		POP		DPH
		POP		DPL
		POP		PSW
		POP		B
		POP		ACC
		SETB	EA
		RETI
		
		
		END
		