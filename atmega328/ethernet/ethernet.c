#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTB1_bit;
sfr sbit SPI_Ethernet_CS  at PORTB2_bit;
sfr sbit SPI_Ethernet_Rst_Direction at DDB1_bit;
sfr sbit SPI_Ethernet_CS_Direction  at DDB2_bit;


unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f};   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150};                     // my IP address
unsigned char   gwIpAddr[4]  = {192, 168, 1, 1};                   // gateway (router) IP address
unsigned char   ipMask[4]    = {255, 255, 255, 0};                   // network mask (for example : 255.255.255.0)
unsigned char   dnsIpAddr[4] = {192, 168, 1, 1};                   // DNS server IP address

void main() {
        SPI1_Init();
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x00);
        SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);
        while(1)
                SPI_Ethernet_doPacket();
}