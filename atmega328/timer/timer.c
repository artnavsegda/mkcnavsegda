void Timer1CompareA_ISR() org IVT_ADDR_TIMER1_COMPA
{
        PORTB5_bit = ~PORTB5_bit;
}

void main()
{
        DDRB = 0xFF;
        WGM12_bit = 1;
        CS12_bit = 1;
        CS10_bit = 1;
        TCNT1L = 0;
        TCNT1H = 0;
        OCR1AH = 0x1E;
        OCR1AL = 0x83;
        OCIE1A_bit = 1;
        asm sei;
        while(1);
}