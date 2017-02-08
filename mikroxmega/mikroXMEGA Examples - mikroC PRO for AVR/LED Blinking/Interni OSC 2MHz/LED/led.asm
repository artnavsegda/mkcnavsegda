
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;led.c,1 :: 		void main() {
;led.c,2 :: 		PORTA_DIRSET = 255;
	LDI        R27, 255
	STS        PORTA_DIRSET+0, R27
;led.c,3 :: 		PORTB_DIRSET = 255;
	LDI        R27, 255
	STS        PORTB_DIRSET+0, R27
;led.c,4 :: 		PORTC_DIRSET = 255;
	LDI        R27, 255
	STS        PORTC_DIRSET+0, R27
;led.c,5 :: 		PORTD_DIRSET = 255;
	LDI        R27, 255
	STS        PORTD_DIRSET+0, R27
;led.c,6 :: 		PORTE_DIRSET = 255;
	LDI        R27, 255
	STS        PORTE_DIRSET+0, R27
;led.c,7 :: 		PORTF_DIRSET = 255;
	LDI        R27, 255
	STS        PORTF_DIRSET+0, R27
;led.c,8 :: 		PORTH_DIRSET = 255;
	LDI        R27, 255
	STS        PORTH_DIRSET+0, R27
;led.c,9 :: 		PORTJ_DIRSET = 255;
	LDI        R27, 255
	STS        PORTJ_DIRSET+0, R27
;led.c,10 :: 		PORTK_DIRSET = 255;
	LDI        R27, 255
	STS        PORTK_DIRSET+0, R27
;led.c,12 :: 		while(1){
L_main0:
;led.c,14 :: 		PORTA_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTA_OUTCLR+0, R27
;led.c,15 :: 		PORTB_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTB_OUTCLR+0, R27
;led.c,16 :: 		PORTC_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTC_OUTCLR+0, R27
;led.c,17 :: 		PORTD_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTD_OUTCLR+0, R27
;led.c,18 :: 		PORTE_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTE_OUTCLR+0, R27
;led.c,19 :: 		PORTF_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTF_OUTCLR+0, R27
;led.c,20 :: 		PORTH_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTH_OUTCLR+0, R27
;led.c,21 :: 		PORTJ_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTJ_OUTCLR+0, R27
;led.c,22 :: 		PORTK_OUTCLR = 255;
	LDI        R27, 255
	STS        PORTK_OUTCLR+0, R27
;led.c,24 :: 		Delay_ms(1000);
	LDI        R18, lo_addr(R11)
	LDI        R17, 38
	LDI        R16, 94
L_main2:
	DEC        R16
	BRNE       L_main2
	DEC        R17
	BRNE       L_main2
	DEC        R18
	BRNE       L_main2
	NOP
	NOP
;led.c,26 :: 		PORTA_OUTSET = 255;
	LDI        R27, 255
	STS        PORTA_OUTSET+0, R27
;led.c,27 :: 		PORTB_OUTSET = 255;
	LDI        R27, 255
	STS        PORTB_OUTSET+0, R27
;led.c,28 :: 		PORTC_OUTSET = 255;
	LDI        R27, 255
	STS        PORTC_OUTSET+0, R27
;led.c,29 :: 		PORTD_OUTSET = 255;
	LDI        R27, 255
	STS        PORTD_OUTSET+0, R27
;led.c,30 :: 		PORTE_OUTSET = 255;
	LDI        R27, 255
	STS        PORTE_OUTSET+0, R27
;led.c,31 :: 		PORTF_OUTSET = 255;
	LDI        R27, 255
	STS        PORTF_OUTSET+0, R27
;led.c,32 :: 		PORTH_OUTSET = 255;
	LDI        R27, 255
	STS        PORTH_OUTSET+0, R27
;led.c,33 :: 		PORTJ_OUTSET = 255;
	LDI        R27, 255
	STS        PORTJ_OUTSET+0, R27
;led.c,34 :: 		PORTK_OUTSET = 255;
	LDI        R27, 255
	STS        PORTK_OUTSET+0, R27
;led.c,36 :: 		Delay_ms(1000);
	LDI        R18, lo_addr(R11)
	LDI        R17, 38
	LDI        R16, 94
L_main4:
	DEC        R16
	BRNE       L_main4
	DEC        R17
	BRNE       L_main4
	DEC        R18
	BRNE       L_main4
	NOP
	NOP
;led.c,37 :: 		}
	JMP        L_main0
;led.c,39 :: 		}
L_end_main:
	JMP        L_end_main
; end of _main
