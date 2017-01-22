#include "average.h"

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_DRDY at PORTC_IN.B1;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

struct massive firststage;

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
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV4, _SPI_CLK_LO_LEADING);
        //SPIC_Init();
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

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main() {
        unsigned int result;
        Sysclk_Init();
        LED0_Direction = 1;
        UARTC0_Init(115200);
        AD7705_Init();
        delay_ms(10);
        AD7705_Write_Bytes("\x20\x0C",2);
        delay_ms(10);
        AD7705_Write_Bytes("\x10\x04", 2);
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
                        increment(&firststage,BSWAP_16(result));
                        
                        PrintOut(PrintHandler, "hex: %5x, ", BSWAP_16(result));
                        PrintOut(PrintHandler, "raw: %5d, ", BSWAP_16(result));
                        PrintOut(PrintHandler, "x16: %5d\r\n", (oversample(&firststage,64)/64));
                        //PrintOut(PrintHandler, "raw: %5d, ", BSWAP_16(result)-0x7FFF);
                        //PrintOut(PrintHandler, "x16: %5d\r\n", (oversample(&firststage,64)/64)-0x7FFF);
                  }
        }
}