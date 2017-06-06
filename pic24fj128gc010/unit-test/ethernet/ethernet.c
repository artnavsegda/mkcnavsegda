#include <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at RD15_bit;
sfr sbit SPI_Ethernet_CS  at RF2_bit;
sfr sbit SPI_Ethernet_Rst_Direction at TRISD15_bit;
sfr sbit SPI_Ethernet_CS_Direction  at TRISF2_bit;

unsigned char   macAddr[6] = {0x00, 0x14, 0xA5, 0x77, 0x18, 0x3f} ;   // my MAC address
unsigned char   ipAddr[4]  = {192, 168, 1, 150} ;                     // my IP address

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void main() {
        unsigned int result;

        TRISA = 0xC6FF;
        TRISB = 0xF0FF;
        TRISC = 0xF01E;
        TRISD = 0x7FFF;
        TRISE = 0x03FF;
        TRISF = 0x3193;
        TRISG = 0xF3CF;

        ANSA = 0xC6F6;
        ANSB = 0xF03F;
        ANSC = 0x001A;
        ANSD = 0xFFFF;
        ANSE = 0x02F0;
        ANSF = 0x21AD;
        ANSG = 0x83C0;

        Unlock_IOLOCK();
        RPOR8bits.RP16R = 0x0008;   //RF3->SPI1:SCK1OUT;
        RPOR8bits.RP17R = 0x0007;   //RF5->SPI1:SDO1;
        RPOR15bits.RP30R = 0x0009;   //RF2->SPI1:SS1OUT;
        RPINR20bits.SDI1R = 0x000A;   //RF4->SPI1:SDI1;
        Lock_IOLOCK();

        Delay_ms(10);
        SPI1_Init();
        SPI_Set_Active(&SPI1_Read, &SPI1_Write);
        SPI_Ethernet_Init(macAddr, ipAddr, 0x01);

        while(1)
        {
                SPI_Ethernet_doPacket();
        }
}