#include <stdint.h>

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sbit SoftSpi_CLK at PORTB7_bit;
sbit SoftSpi_SDI at PINB6_bit;  // Note: Input signal
sbit SoftSpi_SDO at PORTB5_bit;

sbit SoftSpi_CLK_Direction at DDB7_bit;
sbit SoftSpi_SDI_Direction at DDB6_bit;
sbit SoftSpi_SDO_Direction at DDB5_bit;

sfr sbit SPI_DRDY  at PINB4_bit;

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                Soft_SPI_Write(buffer[i]);
}

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = Soft_SPI_Read(0xFF);
}

void main()
{
        uint16_t result;
        char string[10];
        UART1_Init(9600);
        Delay_ms(100);
        UART1_Write_Text("Hello!\r\n");
        Soft_SPI_Init();
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(10);
        SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(10);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);
        while(1)
        {
                if (SPI_DRDY == 0)
                {
                        Soft_SPI_Write(0x38);
                        SPI_Read_Bytes((unsigned char *)&result,2);
                        IntToHex(BSWAP_16(result),string);
                        UART1_Write_Text(string);
                        UART1_Write_Text("\r\n");
                }
        }
}