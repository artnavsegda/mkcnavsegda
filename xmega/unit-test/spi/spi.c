#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_DRDY at PORTC_IN.B1;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        unsigned int result;
        PORTC_DIR.B4 = 1;
        PORTC_OUT.B4 = 0;
        UARTC0_Init(9600);
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV4, _SPI_CLK_LO_LEADING);
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(10);
        SPI_Write_Bytes("\x20\x0C",2);
        delay_ms(10);
        SPI_Write_Bytes("\x10\x04", 2);
        delay_ms(10);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);

        while (1)
        {
                if (AD7705_DRDY == 0)
                {
                        LED0_Toggle = 1;
                	SPI_Write(0x38);
                	SPI_Read_Bytes((unsigned char *)&result,2);
                        PrintOut(PrintHandler, "hex: %5x\r\n", BSWAP_16(result));
                  }
        }
}