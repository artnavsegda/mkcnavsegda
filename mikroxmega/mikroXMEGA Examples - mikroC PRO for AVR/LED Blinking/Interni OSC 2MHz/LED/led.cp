#line 1 "E:/Slavisa/ceves/Support/Testiranje - evidencija/Hardware/2010-09-02/XMEGA-Ready HW REV 1.01/Prilozi/Primeri_za_testiranje/LED Blinking/Interni OSC 2MHz/LED/led.c"
void main() {
 PORTA_DIRSET = 255;
 PORTB_DIRSET = 255;
 PORTC_DIRSET = 255;
 PORTD_DIRSET = 255;
 PORTE_DIRSET = 255;
 PORTF_DIRSET = 255;
 PORTH_DIRSET = 255;
 PORTJ_DIRSET = 255;
 PORTK_DIRSET = 255;

 while(1){

 PORTA_OUTCLR = 255;
 PORTB_OUTCLR = 255;
 PORTC_OUTCLR = 255;
 PORTD_OUTCLR = 255;
 PORTE_OUTCLR = 255;
 PORTF_OUTCLR = 255;
 PORTH_OUTCLR = 255;
 PORTJ_OUTCLR = 255;
 PORTK_OUTCLR = 255;

 Delay_ms(1000);

 PORTA_OUTSET = 255;
 PORTB_OUTSET = 255;
 PORTC_OUTSET = 255;
 PORTD_OUTSET = 255;
 PORTE_OUTSET = 255;
 PORTF_OUTSET = 255;
 PORTH_OUTSET = 255;
 PORTJ_OUTSET = 255;
 PORTK_OUTSET = 255;

 Delay_ms(1000);
 }

}
