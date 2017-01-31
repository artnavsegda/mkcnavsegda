#include  <__EthEnc28j60.h>
#include "modbus.h"
#include "httpUtils.h"
#include "global.h"

#define HTTP_REQUEST_SIZE       128

unsigned int len = 0;
unsigned char httpHeader[100] = "HTTP/1.1 200 OK" ;  // HTTP header
const unsigned char httpMimeTypeHTML[] = "\nContent-type: text/html\n\n";              // HTML MIME type
char webpage[1000];

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        unsigned long filesize, no_bytes;
        static struct mbframestruct askframe;
        static unsigned char getRequest[HTTP_REQUEST_SIZE + 1];
        len = 0;
	switch (localport)
	{
	        case 80:
	        {
	                if(HTTP_getRequest(getRequest, &reqLength, HTTP_REQUEST_SIZE) == 0)
	                        return 0;
                        flags->canCloseTCP = 1;
                 	if (strlen(getRequest) == 1)
                 	        strcpy(getRequest,"/INDEX.HTM");
                        if (sd_init == 0)
                        {
                                if (Mmc_Fat_Assign(&getRequest[1],0) == 1)
                                {
                                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                                        Mmc_Fat_Reset(&filesize);
                                        no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                                        webpage[no_bytes] = 0;
                                }
				else
				{
				        sprintf(httpHeader,"HTTP/1.1 %d Not Found",(int)404);
				        sprintf(webpage,"<!doctype html><html><head><title>404 Not Found</title></head><body><p>%s not found</p></body></html>",getRequest);
				}
				len = SPI_Ethernet_putString(httpHeader);
				len += SPI_Ethernet_putConstString(httpMimeTypeHTML);
				len += SPI_Ethernet_putString(webpage);
				return len;
                        }
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

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}