#include  <__EthEnc28j60.h>
#include "spi.h"
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

void main()
{
        unsigned int result;
        UARTC0_Init(115200);
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(100);
        SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(100);
        
        PORTC_OUT.B4 = 1;
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");

        while (1)
        {
                SPI_Ethernet_doPacket();
                if (AD7707_DRDY == 0)
                {
                        LED0_Toggle = 1;//led0
                        SPIC_Write(0x38);
                        SPIC_Read_Bytes((unsigned char *)&result,2);
                        PrintOut(PrintHandler, "%x\r\n", BSWAP_16(result));
                }
        }
}