
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27
	IN         R28, SPL+0
	IN         R29, SPL+1
	SBIW       R28, 6
	OUT        SPL+0, R28
	OUT        SPL+1, R29
	ADIW       R28, 1

;Uart.c,25 :: 		void main() {
;Uart.c,27 :: 		UARTC0_Init(19200);              // Initialize UART module at 19200 bps
	PUSH       R2
	PUSH       R3
	LDS        R27, USARTC0_CTRLB+0
	SBR        R27, 4
	STS        USARTC0_CTRLB+0, R27
	LDI        R27, 12
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;Uart.c,28 :: 		Delay_ms(1000);                  // Wait for UART module to stabilize
	LDI        R18, lo_addr(R11)
	LDI        R17, 38
	LDI        R16, 94
L_main0:
	DEC        R16
	BRNE       L_main0
	DEC        R17
	BRNE       L_main0
	DEC        R18
	BRNE       L_main0
	NOP
	NOP
;Uart.c,30 :: 		UARTC0_Write_Text("Start");
	LDI        R27, 83
	STD        Y+0, R27
	LDI        R27, 116
	STD        Y+1, R27
	LDI        R27, 97
	STD        Y+2, R27
	LDI        R27, 114
	STD        Y+3, R27
	LDI        R27, 116
	STD        Y+4, R27
	LDI        R27, 0
	STD        Y+5, R27
	MOVW       R16, R28
	MOVW       R2, R16
	CALL       _UARTC0_Write_Text+0
;Uart.c,32 :: 		while (1) {                      // Endless loop
L_main2:
;Uart.c,33 :: 		if (UARTC0_Data_Ready()) {      // If data is received,
	CALL       _UARTC0_Data_Ready+0
	TST        R16
	BRNE       L__main6
	JMP        L_main4
L__main6:
;Uart.c,34 :: 		uart_rd = UARTC0_Read();      // read the received data,
	CALL       _UARTC0_Read+0
	STS        _uart_rd+0, R16
;Uart.c,35 :: 		UARTC0_Write(uart_rd);        // and send data via UART
	MOV        R2, R16
	CALL       _UARTC0_Write+0
;Uart.c,36 :: 		}
L_main4:
;Uart.c,37 :: 		}
	JMP        L_main2
;Uart.c,39 :: 		}
L_end_main:
	JMP        L_end_main
; end of _main
