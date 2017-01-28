#include  <__EthEnc28j60.h>
#include "modbus.h"

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        struct mbframestruct askframe;
        unsigned int len;
	if(localPort == 502)
        {
                SPI_Ethernet_getBytes((unsigned char *)&askframe, 0xFFFF, reqLength);
                flags->canCloseTCP = 0;
                len = modbus(&askframe);
                SPI_Ethernet_putBytes((unsigned char *)&askframe,len);
                return(len);
        }
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return(0);
}

void main() {
        SPIC_Init();
        PORTC_OUT.B4 = 1; //important to disable SPIC SS prior to configure ethernet
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        
        while(1)
                SPI_Ethernet_doPacket();
}