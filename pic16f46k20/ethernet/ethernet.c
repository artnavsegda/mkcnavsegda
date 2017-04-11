#include <__EthEnc28j60.h>
#include "ntp.h"

sfr sbit SPI_Ethernet_Rst at RD2_bit;
sfr sbit SPI_Ethernet_CS  at RD3_bit;
sfr sbit SPI_Ethernet_Rst_Direction at TRISD2_bit;
sfr sbit SPI_Ethernet_CS_Direction  at TRISD3_bit;

unsigned char   macAddr[6] = {0x00, 0x14, 0xA5, 0x77, 0x18, 0x3f} ;   // my MAC address
unsigned char   ipAddr[4]  = {192, 168, 1, 150} ;                     // my IP address

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        struct ntpframestruct ntpframe;
        unsigned int len = 0;
        switch (destPort)
        {
                case 123:
                {
                        if (reqLength > sizeof(ntpframe))
                                reqLength = sizeof(ntpframe);
                        SPI_Ethernet_getBytes((unsigned char *)&ntpframe, 0xFFFF, reqLength);
                        ntp_recieve(&ntpframe);
                }
                break;
                default:
                break;
        }
        return len;
}

void main()
{
        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        SPI1_Init();
        UART1_Write_Text("SPI init\r\n");
        SPI_Ethernet_Init(macAddr, ipAddr, 0x01);
        UART1_Write_Text("Ethernet started\r\n");
        SPI_Ethernet_writeReg(EIE, 0xDB);//enable interrupt flag
        
        while(1)
        {
                SPI_Ethernet_doPacket();
                SPI_Ethernet_writeReg(EIR, 0x00);//clear interrupt flag
        }
}