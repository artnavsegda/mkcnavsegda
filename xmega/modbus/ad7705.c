#include "ad7705.h"

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
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV8, _SPI_CLK_LO_LEADING);
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        AD7705_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(1);
        AD7705_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(1);
        AD7705_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(1);
        AD7705_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(1);
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