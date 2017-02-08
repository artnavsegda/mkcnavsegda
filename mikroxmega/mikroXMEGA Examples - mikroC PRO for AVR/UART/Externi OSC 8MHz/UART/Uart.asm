
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
;Uart.c,28 :: 		OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
	PUSH       R2
	PUSH       R3
	LDI        R27, 64
	STS        OSC_XOSCCTRL+0, R27
;Uart.c,29 :: 		OSC_CTRL |= 0x08;      // enable external oscillator
	LDS        R27, OSC_CTRL+0
	SBR        R27, 8
	STS        OSC_CTRL+0, R27
;Uart.c,31 :: 		while(XOSCRDY_bit == 0)
L_main0:
	LDS        R27, XOSCRDY_bit+0
	SBRC       R27, 3
	JMP        L_main1
;Uart.c,32 :: 		;
	JMP        L_main0
L_main1:
;Uart.c,34 :: 		CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;Uart.c,36 :: 		CLK_CTRL = 3;    // External oscillator
	LDI        R27, 3
	STS        CLK_CTRL+0, R27
;Uart.c,39 :: 		UARTC0_Init(19200);              // Initialize UART module at 19200 bps
	LDS        R16, USARTC0_CTRLB+0
	ANDI       R16, 251
	STS        USARTC0_CTRLB+0, R16
	LDI        R27, 25
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;Uart.c,40 :: 		Delay_ms(1000);                  // Wait for UART module to stabilize
	LDI        R18, 41
	LDI        R17, 150
	LDI        R16, 128
L_main2:
	DEC        R16
	BRNE       L_main2
	DEC        R17
	BRNE       L_main2
	DEC        R18
	BRNE       L_main2
;Uart.c,42 :: 		UARTC0_Write_Text("Start");
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
;Uart.c,44 :: 		while (1) {                      // Endless loop
L_main4:
;Uart.c,45 :: 		if (UARTC0_Data_Ready()) {      // If data is received,
	CALL       _UARTC0_Data_Ready+0
	TST        R16
	BRNE       L__main8
	JMP        L_main6
L__main8:
;Uart.c,46 :: 		uart_rd = UARTC0_Read();      // read the received data,
	CALL       _UARTC0_Read+0
	STS        _uart_rd+0, R16
;Uart.c,47 :: 		UARTC0_Write(uart_rd);        // and send data via UART
	MOV        R2, R16
	CALL       _UARTC0_Write+0
;Uart.c,48 :: 		}
L_main6:
;Uart.c,49 :: 		}
	JMP        L_main4
;Uart.c,51 :: 		}
L_end_main:
	JMP        L_end_main
; end of _main
