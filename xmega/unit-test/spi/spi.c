#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
sfr sbit LED0 at PORTR_OUT.B0;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
}

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPIC_Read(0xFF);
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
}

void SPIC_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPIC_Write(buffer[i]);
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        unsigned int result;
        LED0_Direction = 1;//led0
        PORTC_DIR.B4 = 1;
        PORTCFG_CLKEVOUT.CLKEVPIN = 1;
        PORTCFG_CLKEVOUT.CLKOUT0 = 1;
        UARTC0_Init(9600);
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV8, _SPI_CLK_LO_LEADING);
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(100);
        SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(100);

        while (1)
        {
		LED0_Toggle = 1;//led0
		SPIC_Write(0x38);
		SPIC_Read_Bytes((unsigned char *)&result,2);
		PrintOut(PrintHandler, "%x\r\n", BSWAP_16(result));
		delay_ms(100);
        }
}