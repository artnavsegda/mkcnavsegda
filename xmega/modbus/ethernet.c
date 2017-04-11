#include  <__EthEnc28j60.h>
#include "ethernet.h"
#include "modbus.h"
#include "global.h"
#include "http.h"
#include "tftp.h"
#include "ntp.h"

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        static mbframestruct askframe;
        static unsigned char getRequest[1500];
        unsigned char *method;
        unsigned char *page;
        unsigned char *buf2;
        len = 0;
        switch (localport)
        {
                case 80:
                {
                        if (reqLength > sizeof(getRequest))
                                reqLength = sizeof(getRequest);
                        SPI_Ethernet_getBytes(getRequest, 0xFFFF, reqLength);
                        getRequest[reqLength] = 0;
                        method = strtok(getRequest," ");
                        page = strtok(0," ");
                        buf2 = page+strlen(page)+1;//ugly but well fuck
                        flags->canCloseTCP = 1;
                        len = http(page,buf2);
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
        static struct ntpframestruct ntpframe;
        switch (destPort)
        {
                case 69:
                        len = 0;
                        len = tftp(reqLength);
                break;
                case 123:
                        if (reqLength > sizeof(ntpframe))
                                reqLength = sizeof(ntpframe);
                        SPI_Ethernet_getBytes((unsigned char *)&ntpframe, 0xFFFF, reqLength);
                        ntp_recieve(&ntpframe);
                break;
                default:
                        len = 0;
                break;
        }
        return len;
}