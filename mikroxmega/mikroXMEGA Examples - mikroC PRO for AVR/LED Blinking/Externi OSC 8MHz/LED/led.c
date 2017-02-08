void main() {
// ----- External Clock selection procedure!!!
   OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
   OSC_CTRL |= 0x08;      // enable external oscillator

   while(XOSCRDY_bit == 0)
    ;

   CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
                    // I/O register or execution of the protected instruction for a maximum of 4 CPU instruction cycles.
   CLK_CTRL = 3;    // External oscillator
// ---------------------------------


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