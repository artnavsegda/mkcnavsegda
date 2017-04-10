#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B1;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main() {
        Sysclk_Init();
        UARTC0_Init(9600);
        Delay_ms(10);
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init();
        PORTC_OUT.B4 = 1; //important to disable SPIC SS prior to configure ethernet
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        UARTC0_Write_Text("Ethernet started\r\n");

        while(1)
                SPI_Ethernet_doPacket();
}