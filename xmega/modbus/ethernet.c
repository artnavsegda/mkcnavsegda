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
        return 0;
}
