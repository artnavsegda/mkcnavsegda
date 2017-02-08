
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;led.c,1 :: 		void main() {
;led.c,3 :: 		OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
	LDI        R27, 64
	STS        OSC_XOSCCTRL+0, R27
;led.c,4 :: 		OSC_CTRL |= 0x08;      // enable external oscillator
	LDS        R27, OSC_CTRL+0
	SBR        R27, 8
	STS        OSC_CTRL+0, R27
;led.c,6 :: 		while(XOSCRDY_bit == 0)
L_main0:
	LDS        R27, XOSCRDY_bit+0
	SBRC       R27, 3
	JMP        L_main1
;led.c,7 :: 		;
	JMP        L_main0
L_main1:
;led.c,9 :: 		CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;led.c,11 :: 		CLK_CTRL = 3;    // External oscillator
	LDI        R27, 3
	STS        CLK_CTRL+0, R27
;led.c,15 :: 		PORTA_DIRSET = 255;
	LDI        R27, 255
	STS        PORTA_DIRSET+0, R27
;led.c,16 :: 		PORTB_DIRSET = 255;
	LDI        R27, 255
	STS        PORTB_DIRSET+0, R27
;led.c,17 :: 		PORTC_DIRSET = 255;
	LDI        R27, 255
	STS        PORTC_DIRSET+0, R27
;led.c,18 :: 		PORTD_DIRSET = 255;
	LDI        R27, 255
	STS        PORTD_DIRSET+0, R27
;led.c,19 :: 		PORTE_DIRSET = 255;
	LDI        R27, 255
	STS        PORTE_DIRSET+0, R27
;led.c,20 :: 		PORTF_DIRSET = 255;
	LDI        R27, 255
	STS        PORTF_DIRSET+0, R27
;led.c,21 :: 		PORTH_DIRSET = 255;
	LDI        R27, 255
	STS        PORTH_DIRSET+0, R27
;led.c,22 :: 		PORTJ_DIRSET = 255;
	LDI        R27, 255
	STS        PORTJ_DIRSET+0, R27
;led.c,23 :: 		PORTK_DIRSET = 255;
	LDI        R27, 255
	STS        PORTK_DIRSET+0, R27
;led.c,25 :: 		while(1){
L_main2:
;led.c,27 :: 		PORTA_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTA_OUTCLR+0, R27
;led.c,28 :: 		PORTB_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTB_OUTCLR+0, R27
;led.c,29 :: 		PORTC_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTC_OUTCLR+0, R27
;led.c,30 :: 		PORTD_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTD_OUTCLR+0, R27
;led.c,31 :: 		PORTE_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTE_OUTCLR+0, R27
;led.c,32 :: 		PORTF_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTF_OUTCLR+0, R27
;led.c,33 :: 		PORTH_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTH_OUTCLR+0, R27
;led.c,34 :: 		PORTJ_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTJ_OUTCLR+0, R27
;led.c,35 :: 		PORTK_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTK_OUTCLR+0, R27
;led.c,37 :: 		Delay_ms(1000);
	LDI        R18, 41
	LDI        R17, 150
	LDI        R16, 128
L_main4:
	DEC        R16
	BRNE       L_main4
	DEC        R17
	BRNE       L_main4
	DEC        R18
	BRNE       L_main4
;led.c,39 :: 		PORTA_OUTSET = 255;
	LDI        R27, 255
	STS        PORTA_OUTSET+0, R27
;led.c,40 :: 		PORTB_OUTSET = 255;
	LDI        R27, 255
	STS        PORTB_OUTSET+0, R27
;led.c,41 :: 		PORTC_OUTSET = 255;
	LDI        R27, 255
	STS        PORTC_OUTSET+0, R27
;led.c,42 :: 		PORTD_OUTSET = 255;
	LDI        R27, 255
	STS        PORTD_OUTSET+0, R27
;led.c,43 :: 		PORTE_OUTSET = 255;
	LDI        R27, 255
	STS        PORTE_OUTSET+0, R27
;led.c,44 :: 		PORTF_OUTSET = 255;
	LDI        R27, 255
	STS        PORTF_OUTSET+0, R27
;led.c,45 :: 		PORTH_OUTSET = 255;
	LDI        R27, 255
	STS        PORTH_OUTSET+0, R27
;led.c,46 :: 		PORTJ_OUTSET = 255;
	LDI        R27, 255
	STS        PORTJ_OUTSET+0, R27
;led.c,47 :: 		PORTK_OUTSET = 255;
	LDI        R27, 255
	STS        PORTK_OUTSET+0, R27
;led.c,49 :: 		Delay_ms(1000);
	LDI        R18, 41
	LDI        R17, 150
	LDI        R16, 128
L_main6:
	DEC        R16
	BRNE       L_main6
	DEC        R17
	BRNE       L_main6
	DEC        R18
	BRNE       L_main6
;led.c,50 :: 		}
	JMP        L_main2
;led.c,52 :: 		}
L_end_main:
	JMP        L_end_main
; end of _main
