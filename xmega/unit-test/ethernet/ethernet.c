#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B4;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B1;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B4;

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f};   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150};                     // my IP address
unsigned char   gwIpAddr[4]  = {192, 168, 1, 1};                   // gateway (router) IP address
unsigned char   ipMask[4]    = {255, 255, 255, 0};                   // network mask (for example : 255.255.255.0)
unsigned char   dnsIpAddr[4] = {192, 168, 1, 1};                   // DNS server IP address

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
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

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        SPI_Ethernet_userTimerSec++;
}

void PrintHandler2(char c)
{
        UARTC0_Write(c);
}

void main() {
        int result = 3;
        Sysclk_Init();
        UARTC0_Init(9600);
        Delay_ms(10);
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init();
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x00);
        SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);
        UARTC0_Write_Text("Ethernet started\r\n");
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        PMIC_CTRL.HILVLEN = 1;
        SREG_I_bit = 1;
        UARTC0_Write_Text("Timer started\r\n");

        while(1)
                SPI_Ethernet_doPacket();
}