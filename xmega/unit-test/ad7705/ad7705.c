#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_DRDY at PORTC_IN.B1;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

void AD7705_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
        AD7705_CS = 1;
}

void AD7705_Init(void)
{
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        AD7705_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
}

void AD7705_Read_Register(unsigned char addr, unsigned char *value, unsigned int NoBytes)
{
        int i;
        AD7705_CS = 0;
        SPIC_Write(addr);
        for (i = 0; i < NoBytes; i++)
                value[i] = SPIC_Read(0xFF);
        AD7705_CS = 1;
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
	unsigned int result;
	LED0_Direction = 1;
        UARTC0_Init(115200);
        AD7705_Init();
        delay_ms(10);
        AD7705_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);
        
        while (1)
        {
                if (AD7705_DRDY == 0)
		{
		        LED0_Toggle = 1;
		        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
		        PrintOut(PrintHandler, "%x\r\n", BSWAP_16(result));
  		}
	}
}