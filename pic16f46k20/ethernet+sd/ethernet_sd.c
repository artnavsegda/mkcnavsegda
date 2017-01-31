#include  <__EthEnc28j60.h>
#include "httpUtils.h"

#define HTTP_REQUEST_SIZE       128
#define putConstString  SPI_Ethernet_putConstString
#define putString  SPI_Ethernet_putString

sfr sbit Mmc_Chip_Select           at LATB0_bit;  // for writing to output pin always use latch (PIC18 family)
sfr sbit Mmc_Chip_Select_Direction at TRISB0_bit;
sfr sbit SPI_Ethernet_Rst at LATD2_bit;
sfr sbit SPI_Ethernet_CS  at LATD3_bit;
sfr sbit SPI_Ethernet_Rst_Direction at TRISD2_bit;
sfr sbit SPI_Ethernet_CS_Direction  at TRISD3_bit;

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address
int    len = 0;            // my reply length
unsigned char httpHeader[100] = "HTTP/1.1 200 OK";  // HTTP header
const unsigned char httpMimeTypeHTML[] = "\nContent-type: text/html\n\n";              // HTML MIME type
char sd_init = 9, sd_format, sd_exists, sd_assign = 9;
char webpage[1000];

void WebHandler(char c)
{
        SPI_Ethernet_putByte(c);
        len++;
}

void PrintHandler(char c)
{
        UART1_Write(c);
}

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        unsigned long filesize, no_bytes;
        unsigned char   getRequest[HTTP_REQUEST_SIZE + 1];
        flags->canCloseTCP = 1;
        len = 0;
        if (localPort != 80)
                return 0;
        if(HTTP_getRequest(getRequest, &reqLength, HTTP_REQUEST_SIZE) == 0)
                return 0;
        if (strlen(getRequest) == 1)
        {
                UART1_Write_Text("Index page\r\n");
                strcpy(getRequest,"/INDEX.HTM");
        }
        UART1_Write_Text(getRequest);
        UART1_Write_Text("\r\n");
        if (sd_init == 0)
        {
                sd_assign = Mmc_Fat_Assign(&getRequest[1],0);
                if (sd_assign == 1)
                {
                        UART1_Write_Text("File open valid\r\n");
                        sprintf(httpHeader,"HTTP/1.1 %d OK",(int)200);
                        Mmc_Fat_Reset(&filesize);
                        PrintOut(PrintHandler, "file size is %d\r\n", filesize);
                        no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                        webpage[no_bytes] = 0;
                        PrintOut(PrintHandler, "read %d bytes from file\r\n", no_bytes);
                        PrintOut(PrintHandler, "assumed %d text length\r\n",strlen(webpage));
                }
                else
                {
                        UART1_Write_Text("File not found\r\n");
			sprintf(httpHeader,"HTTP/1.1 %d Not Found",(int)404);
			sprintf(webpage,"<!doctype html><html><head><title>404 Not Found</title></head><body><p>%s not found</p></body></html>",getRequest);
                }
                len = putString(httpHeader);
                len += putConstString(httpMimeTypeHTML);
                len += putString(webpage);
        }
        return len;
}

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void main()
{
        UART1_Init(115200);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        //TRISD3_bit = 0; //set ethernet cs as output
        SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
        //LATD3_bit = 1; //shut up ethernet controller

        sd_init = Mmc_Fat_Init();

        switch(sd_init)
        {
                case 0:
                        UART1_Write_Text("SD FAT Init-OK\r\n");
                break;
                case 1:
                        UART1_Write_Text("SD FAT not found\r\n");
                break;
                default:
                        UART1_Write_Text("SD Init-error\r\n");
                break;
        }
        
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x01);

        while(1)
        {
                SPI_Ethernet_doPacket();
        }
}