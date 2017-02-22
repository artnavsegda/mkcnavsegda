#include  <__EthEnc28j60.h>
#include "ad7705.h"
#include "ports.h"

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
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

void main()
{
        unsigned int result;
        LED0_Direction = 1;
        Sysclk_Init();
        UARTC0_Init(115200);
        AD7705_Init();
        delay_ms(10);
        AD7705_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);
        
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");

        while (1)
        {
                SPI_Ethernet_doPacket();
                if (AD7707_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                        PrintOut(PrintHandler, "%x\r\n", BSWAP_16(result));
                }
        }
}