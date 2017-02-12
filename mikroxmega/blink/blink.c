void main()
{
	// ----- External Clock selection procedure!!!
	OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
	OSC_CTRL |= 0x08;      // enable external oscillator
	while(XOSCRDY_bit == 0);
	CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
			 // I/O register or execution of the protected instruction for a maximum of 4 CPU instruction cycles.
	CLK_CTRL = 3;    // External oscillator
	// ---------------------------------

        PORTA_DIR = 0xFF;
        PORTB_DIR = 0xFF;
        PORTC_DIR = 0xFF;
        PORTA_OUT = 0xFF;
        PORTB_OUT = 0xFF;
        PORTC_OUT = 0xFF;

        while (1)
        {
                delay_ms(300);
                PORTA_OUTTGL = 0xFF;
                delay_ms(300);
                PORTB_OUTTGL = 0xFF;
                delay_ms(300);
                PORTC_OUTTGL = 0xFF;
        }
}