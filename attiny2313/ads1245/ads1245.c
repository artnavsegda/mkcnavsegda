#include <stdint.h>

void starttimer(void)
{
        TCCR0A = 0x33;
        TCCR0B = 0x09;
        OCR0A = 0x06;
        OCR0B = 0x03;
}

uint32_t shiftIn(void)
{
        uint32_t value = 0;
        uint8_t i;
        for (i = 0;i < 24; ++i)
        {
                PORTD4_bit = 1;
                value |= PIND3_bit << (24-i);
                PORTD4_bit = 0;
        }
        PORTD4_bit = 1;
        PORTD4_bit = 0;// 25th pulse to keep DATA high till next data ready
        return value;
}

void main()
{
        char string[10];
        DDD5_bit = 1; //clk
        DDD4_bit = 1; //sck
        UART1_Init(9600);
        starttimer();

        while(1)
        {
                if (PIND3_bit == 0)
                {
                        LongIntToHex(shiftIn(),string);
                        UART1_Write_Text(string);
                        UART1_Write_Text("\r\n");
                }
        }
}