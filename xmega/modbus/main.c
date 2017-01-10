#include  <__EthEnc28j60.h>
#include "ad7705.h"
#include "ports.h"
#include "bswap.h"

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main()
{
        unsigned int result;
        LED0_Direction = 1;
        UARTC0_Init(115200);
        AD7705_Init();
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");

        while (1)
        {
                SPI_Ethernet_doPacket();
                if (AD7707_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                }
        }
}