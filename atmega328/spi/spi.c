#include <stdint.h>

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit SPI_AD7705_CS  at PORTB2_bit;
sfr sbit SPI_DRDY  at PINB1_bit;

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        SPI_AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
        SPI_AD7705_CS = 1;
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        SPI_AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
        SPI_AD7705_CS = 1;
}

void PrintHandler(char c)
{
        UART1_Write(c);
}

void main() {
        uint16_t result;
        UART1_Init(9600);
        delay_ms(100);
        UART1_Write_Text("MCU started\r\n");
        SPI1_Init();
        SPI_Set_Active(&SPI1_Read, &SPI1_Write);
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
                if (SPI_DRDY == 0)
                {
                	SPI_AD7705_CS = 0;
                        SPI_Write(0x38);
                        SPI_AD7705_CS = 1;
                        SPI_Read_Bytes((unsigned char *)&result,2);
                        PrintOut(PrintHandler, "hex: %04x\r\n", BSWAP_16(result));
                }
        }
}