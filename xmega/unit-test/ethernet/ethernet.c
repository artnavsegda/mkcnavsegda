#include  <__EthEnc28j60.h>
#include "modbus.h"
#include "ntp.h"

unsigned int len = 0;

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        static struct mbframestruct askframe;
        static unsigned char getRequest[1500];
        len = 0;
        switch (localport)
        {
                case 502:
                {
                        if (reqLength > sizeof(askframe))
                                reqLength = sizeof(askframe);
                        SPI_Ethernet_getBytes((unsigned char *)&askframe, 0xFFFF, reqLength);
                        flags->canCloseTCP = 0;
                        len = modbus(&askframe);
                        SPI_Ethernet_putBytes((unsigned char *)&askframe,len);
                        return len;
                }
                break;
                default:
                break;
         }
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        struct ntpframestruct ntpframe;
        //UARTC0_Write_Text("UDP package recieved\r\n");
        len = 0;
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

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

unsigned char IpAddr[4]  = {192, 168,   1,  113 };  // remote IP address

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
        //PORTC_OUT.B4 = 1; //important to disable SPIC SS prior to configure ethernet
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x00);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        UARTC0_Write_Text("Ethernet started\r\n");
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        PMIC_CTRL.HILVLEN = 1;
        CPU_SREG.I = 1;
        UARTC0_Write_Text("Timer started\r\n");
        SPI_Ethernet_arpResolve(IpAddr, 5);
        UARTC0_Write_Text("ARP resolved\r\n");
        result = SPI_Ethernet_sendUDP(IpAddr, 100, 100, "Hello", 5);
        UARTC0_Write_Text("UDP package sent\r\n");
        PrintOut(PrintHandler2, "result: %d\r\n",result);
        ntp_send();
        UARTC0_Write_Text("NTP package sent\r\n");
        
        while(1)
                SPI_Ethernet_doPacket();
}