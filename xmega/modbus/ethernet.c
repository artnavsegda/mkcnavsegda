#include  <__EthEnc28j60.h>
#include "modbus.h"
#include "httpUtils.h"
#include "global.h"
#include "bswap.h"

#define HTTP_REQUEST_SIZE       128

unsigned int len = 0;
unsigned char httpHeader[100] = "HTTP/1.1 200 OK" ;  // HTTP header
const char * httpMimeType;
const char httpMimeTypeHTML[] = "\nContent-type: text/html\n\n" ;              // HTML MIME type
const char httpMimeTypeScript[] = "\nContent-type: application/javascript\n\n" ;           // JS MIME type
const char httpMimeTypeText[] = "\nContent-type: text/plain\n\n" ;           // TEXT MIME type
const char httpMimeTypeCSS[] = "\nContent-type: text/CSS\n\n" ;           // CSS MIME type
char webpage[1000];

void WebHandler(char c)
{
        SPI_Ethernet_putByte(c);
        len++;
}

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
                                 strcpy(getRequest,"/index.htm");
                        if (sd_init == 0)
                        {
                                if (strcmp("/dir.htm",getRequest)==0)
                                {
                                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                                        len = SPI_Ethernet_putString(httpHeader);
                                        len += SPI_Ethernet_putConstString(httpMimeTypeHTML);
                                        len += SPI_Ethernet_putConstString("<!doctype html><html><head><title>File list</title></head><body><pre>");
                                        Mmc_Fat_Dir(WebHandler);
                                        len += SPI_Ethernet_putConstString("</pre></body></html>");
                                }
                                else
                                {
                                        if (Mmc_Fat_Assign(&getRequest[1],0) == 1)
                                        {
                                                sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                                                Mmc_Fat_Reset(&filesize);
                                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                                                webpage[no_bytes] = 0;
                                                if (strcmp(strchr(getRequest,'.'),".htm")==0)
                                                        httpMimeType = httpMimeTypeHTML;
                                                else if (strcmp(strchr(getRequest,'.'),".js")==0)
                                                        httpMimeType = httpMimeTypeScript;
                                                else if (strcmp(strchr(getRequest,'.'),".txt")==0)
                                                        httpMimeType = httpMimeTypeText;
                                                else if (strcmp(strchr(getRequest,'.'),".css")==0)
                                                        httpMimeType = httpMimeTypeCSS;
                                        }
                                        else
                                        {
                                                sprintf(httpHeader,"HTTP/1.1 %d Not Found",(int)404);
                                                httpMimeType = httpMimeTypeHTML;
                                                sprintf(webpage,"<!doctype html><html><head><title>404 Not Found</title></head><body><p>%s not found</p></body></html>",getRequest);
                                        }
                                        len = SPI_Ethernet_putString(httpHeader);
                                        len += SPI_Ethernet_putConstString(httpMimeType);
                                        len += SPI_Ethernet_putString(webpage);
                                }
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

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        static unsigned long filesize;
        unsigned int no_bytes;
        static int block = 0;
        int errcode;
        int opcode = 0;
        if (destPort != 69)
                return 0;
        len = 0;
        SPI_Ethernet_getBytes((unsigned char *)&opcode,0xFFFF,2);
        switch(BSWAP_16(opcode))
        {
                case 1:
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-2);
                        if (sd_init != 0)
                        {
                                opcode = BSWAP_16(5);
                                SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                len = 2;
                                errcode = 3;//BSWAP_16(3);
                                SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                len += 2;
                                len += SPI_Ethernet_putString("SD disfunctional");
                                SPI_Ethernet_putByte(0);
                                len++;
                        }
                        else
                        {
                                if (Mmc_Fat_Assign(webpage,0) != 1)
                                {
                                        opcode = BSWAP_16(5);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        errcode = 1;//BSWAP_16(1);
                                        SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                        len += 2;
                                        len += SPI_Ethernet_putString("File not present");
                                        SPI_Ethernet_putByte(0);
                                        len++;
                                }
                                else
                                {
                                        Mmc_Fat_Reset(&filesize);
                                        if (filesize < 512)
                                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                                        else
                                                no_bytes = Mmc_Fat_ReadN(webpage, 512);
                                        opcode = BSWAP_16(3);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        block = BSWAP_16(1);
                                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                                        len += 2;
                                        SPI_Ethernet_putBytes(webpage,no_bytes);
                                        len +=no_bytes;
                                }
                        }
                break;
                case 2:
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-2);
                        if (sd_init != 0)
                        {
                                opcode = BSWAP_16(5);
                                SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                len = 2;
                                len += SPI_Ethernet_putString("SD disfunctional\0");
                                SPI_Ethernet_putByte(0);
                                len++;
                        }
                        else
                        {
                                if (Mmc_Fat_Assign(webpage,0x80) != 1)
                                {
                                        opcode = BSWAP_16(5);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        errcode = 6;//BSWAP_16(6);
                                        SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                        len += 2;
                                        len += SPI_Ethernet_putString("File creation failed");
                                        SPI_Ethernet_putByte(0);
                                        len++;
                                }
                                else
                                {
                                        Mmc_Fat_Rewrite();
                                        opcode = BSWAP_16(4);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        block = BSWAP_16(0);
                                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                                        len += 2;
                                }
                        }
                break;
                case 3:
                        SPI_Ethernet_getBytes((unsigned char *)&block,0xFFFF,2);
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-4);
                        Mmc_Fat_Write(webpage,reqLength-4);
                        opcode = BSWAP_16(4);
                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                        len = 2;
                        //block = BSWAP_16(BSWAP_16(block)+1);
                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                        len += 2;
                        if (reqLength-4 != 512)
                                Mmc_Fat_Close();
                break;
                case 4:
                        SPI_Ethernet_getBytes((unsigned char *)&block,0xFFFF,2);
                        filesize = filesize - 512;
                        if (filesize < 512)
                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                        else
                                no_bytes = Mmc_Fat_ReadN(webpage, 512);
                        opcode = BSWAP_16(3);
                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                        len = 2;
                        block = BSWAP_16(BSWAP_16(block)+1);
                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                        len += 2;
                        SPI_Ethernet_putBytes(webpage,no_bytes);
                        len +=no_bytes;
                break;
        }
        return len;
}