#include <__EthEnc28j60.h>

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
        return 0;
}

void main() {
        unsigned int result;

        TRISA = 0xC6FF;
        TRISB = 0xFFFF;
        TRISC = 0x901E;
        TRISD = 0xFFFF;
        TRISE = 0x03FF;
        TRISF = 0x31B7;
        TRISG = 0xF28F;

        ANSA = 0xC6ED;
        ANSB = 0xFFFC;
        ANSC = 0x001E;
        ANSD = 0xFFFF;
        ANSE = 0x03FF;
        ANSF = 0x313B;
        ANSG = 0xF343;
        ANSH = 0x001F;

        Unlock_IOLOCK();
        RPOR9bits.RP19R = 0x0007;   //RG8->SPI1:SDO1;
        RPOR8bits.RP16R = 0x0003;   //RF3->UART1:U1TX;
        RPOR10bits.RP21R = 0x0008;   //RG6->SPI1:SCK1OUT;
        RPINR18bits.U1RXR = 0x001E;   //RF2->UART1:U1RX;
        RPINR20bits.SDI1R = 0x001A;   //RG7->SPI1:SDI1;
        Lock_IOLOCK();

        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("Hello world\r\n");
        SPI1_Init();
        SPI_Set_Active(&SPI1_Read, &SPI1_Write);
        SPI_Ethernet_Init(macAddr, ipAddr, 0x01);

        while(1)
        {
                SPI_Ethernet_doPacket();
        }
}