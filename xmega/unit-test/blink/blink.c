unsigned char FUBAR;
sbit FUBAR_first at FUBAR.B0;

void main() {
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