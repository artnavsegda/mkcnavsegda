#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
sfr sbit AD7705_DRDY at PORTC_IN.B1;
sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit LED0 at PORTR_OUT.B0;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
        AD7705_CS = 1;
}

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPIC_Read(0xFF);
        AD7705_CS = 1;
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
        AD7705_CS = 1;
}

void SPIC_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                SPIC_Write(buffer[i]);
        AD7705_CS = 1;
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main() {
        unsigned int result;
        LED0_Direction = 1;//led0
        Sysclk_Init();
        UARTC0_Init(115200);
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
                if (AD7705_DRDY == 0)
                {
                        LED0_Toggle = 1;//led0
                        AD7705_CS = 0;
                        SPIC_Write(0x38);
                        SPIC_Read_Bytes((unsigned char *)&result,2);
                        AD7705_CS = 1;
                        PrintOut(PrintHandler, "%x\r\n", BSWAP_16(result));
                }
        }
}