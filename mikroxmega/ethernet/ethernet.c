#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B3;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B4;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B3;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B4;

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void SysClk_Init() {
	// ----- External Clock selection procedure!!!
	OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
	OSC_CTRL |= 0x08;      // enable external oscillator
	while(XOSCRDY_bit == 0);
	CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
        	         // I/O register or execution of the protected instruction for a maximum of 4 CPU instruction cycles.
	CLK_CTRL = 3;    // External oscillator
	// ---------------------------------
}

void main() {
	SysClk_Init();
        SPIC_Init();
        //PORTC_OUT.B4 = 1; //important to disable SPIC SS prior to configure ethernet
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");

        while(1)
                SPI_Ethernet_doPacket();
}