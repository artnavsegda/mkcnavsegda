#include  <__EthEnc28j60.h>
#include "httpUtils.h"

#define HTTP_REQUEST_SIZE       128

#define putConstString  SPI_Ethernet_putConstString
#define putString  SPI_Ethernet_putString

sfr sbit SPI_Ethernet_Rst at RD2_bit;
sfr sbit SPI_Ethernet_CS  at RD3_bit;
sfr sbit SPI_Ethernet_Rst_Direction at TRISD2_bit;
sfr sbit SPI_Ethernet_CS_Direction  at TRISD3_bit;

const unsigned char httpHeader[] = "HTTP/1.1 200 OK\nContent-type: " ;  // HTTP header
const unsigned char httpMimeTypeHTML[] = "text/html\n\n" ;              // HTML MIME type
const unsigned char httpMimeTypeScript[] = "text/plain\n\n" ;           // TEXT MIME type

/*
 * HTTP + HTML common header
 */
const char    *HTMLheader = "\
<HTML><HEAD>\
<TITLE>MikroElektronika</TITLE>\
</HEAD><BODY>\
<h2>MikroElektronika</h2>\
" ;

/*
 * HTML common footer
 */
const   char    *HTMLfooter = "\
</BODY></HTML>\n\n" ;

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address
unsigned int    len = 0;            // my reply length

void PrintHandler(char c)
{
	SPI_Ethernet_putByte(c);
	len++;
}

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        unsigned char   getRequest[HTTP_REQUEST_SIZE + 1];
        len = 0;
        if(localPort != 80)                     // I listen only to web request on port 80
        {
                return(0);                     // return without reply
        }
        if(HTTP_getRequest(getRequest, &reqLength, HTTP_REQUEST_SIZE) == 0)
        {
                return(0);                     // no reply if no GET request
        }
        len = putConstString(httpHeader);              // HTTP header
        len += putConstString(httpMimeTypeHTML);     // with text MIME type
        len += putConstString(HTMLheader);
        //len += putConstString("Hello, world !");
        if (strncmp(getRequest, "/ ", 2) == 0)
        	PrintOut(PrintHandler,"Index page");
	else
	        PrintOut(PrintHandler,"Not Found");
        //len += putString(getRequest);
        len += putConstString(HTMLfooter);
        return len;
}

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void main()
{
        SPI1_Init();
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x01);
        
        while(1)
        {
                SPI_Ethernet_doPacket();
        }
}