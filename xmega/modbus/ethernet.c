#include  <__EthEnc28j60.h>
#include "modbus.h"
#include "global.h"
#include "http.h"
#include "tftp.h"

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        static struct mbframestruct askframe;
        unsigned char *getRequest;
        unsigned char *method;
        unsigned char *page;
        unsigned char *buf2;
        len = 0;
        switch (localport)
        {
                case 80:
                {
                        getRequest = Malloc(reqLength+1);
                        SPI_Ethernet_getBytes(getRequest, 0xFFFF, reqLength);
                        getRequest[reqLength] = 0;
                        method = strtok(getRequest," ");
                        page = strtok(0," ");
                        buf2 = page+strlen(page)+1;//ugly but well fuck
                        flags->canCloseTCP = 1;
                        len = http(page,buf2);
                        Free(getRequest,reqLength+1);
                        return len;
                }
                break;
                case 502:
                {
                        if (reqLength > sizeof(askframe))
                                reqLength = sizeof(askframe);
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