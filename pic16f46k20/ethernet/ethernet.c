#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at RD2_bit;
sfr sbit SPI_Ethernet_CS  at RD3_bit;
sfr sbit SPI_Ethernet_Rst_Direction at TRISD2_bit;
sfr sbit SPI_Ethernet_CS_Direction  at TRISD3_bit;

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
	return 0;
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
