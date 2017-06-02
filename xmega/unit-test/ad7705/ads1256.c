sfr sbit ADS1256_CS at PORTC_OUT.B4;
sfr sbit ADS1256_DRDY at PORTC_IN.B1;
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
        unsigned long result;
        PORTC_DIR.B4 = 1;
        PORTC_OUT.B4 = 0;
        UARTC0_Init(9600);
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV4, _SPI_CLK_LO_LEADING);
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        SPI_Write(0xFE); //reset
        delay_ms(10);
        SPI_Write_Bytes("\x53\x03",2);//write to data rate register sample rate 2.5sps
        delay_ms(10);

        while (1)
        {
                if (ADS1256_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        SPI_Write(0x01);//read data command
                        SPI_Read_Bytes((unsigned char *)&result,3);
                        PrintOut(PrintHandler, "hex: %5x\r\n", result);
                  }
        }
}
