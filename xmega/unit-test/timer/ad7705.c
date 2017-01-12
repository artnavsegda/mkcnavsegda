sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_DRDY at PORTC_IN.B1;

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