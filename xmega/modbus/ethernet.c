#include  <__EthEnc28j60.h>
#include "modbus.h"
#include "httpUtils.h"
#include "global.h"
#include "http.h"
#include "tftp.h"

#define HTTP_REQUEST_SIZE       1410

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        static struct mbframestruct askframe;
        static unsigned char getRequest[HTTP_REQUEST_SIZE + 1];
        unsigned char *method;
        unsigned char *page;
        unsigned char *buf2;
        len = 0;
        switch (localport)
        {
                case 80:
                {
                        SPI_Ethernet_getBytes(getRequest, 0xFFFF, reqLength);
                        getRequest[reqLength] = 0;
			method = strtok(getRequest," ");
			page = strtok(0," ");
			buf2 = strtok(0,"");
                        //if(HTTP_getRequest(getRequest, &reqLength, HTTP_REQUEST_SIZE) == 0)
                        //        return 0;
                        flags->canCloseTCP = 1;
                        len = http(page,buf2);
                        return len;
                }
                break;
                case 502:
                {
                        SPI_Ethernet_getBytes((unsigned char *)&askframe, 0xFFFF, reqLength);
                        flags->canCloseTCP = 0;
                        len = modbus(&askframe);
                        SPI_Ethernet_putBytes((unsigned char *)&askframe,len);
                        return len;
                }
                break;
                default:
                break;
         }
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        switch (destPort)
        {
                case 69:
                        len = 0;
                        len = tftp(reqLength);
                break;
                default:
                        len = 0;
                break;
        }
        return len;
}