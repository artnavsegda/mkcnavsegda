#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTB4_bit;
sfr sbit SPI_Ethernet_CS  at PORTB5_bit;
sfr sbit SPI_Ethernet_Rst_Direction at DDB4_bit;
sfr sbit SPI_Ethernet_CS_Direction  at DDB5_bit;


unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void main() {
        SPI1_Init();
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x00);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        while(1)
                SPI_Ethernet_doPacket();
}