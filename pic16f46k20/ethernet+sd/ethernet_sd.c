#include  <__EthEnc28j60.h>

sfr sbit Mmc_Chip_Select           at LATB0_bit;  // for writing to output pin always use latch (PIC18 family)
sfr sbit Mmc_Chip_Select_Direction at TRISB0_bit;
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
        char sd_init, sd_format, sd_exists, sd_assign;
        UART1_Init(115200);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        //TRISD3_bit = 0; //set ethernet cs as output
        SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
        //LATD3_bit = 1; //shut up ethernet controller

        sd_init = Mmc_Fat_Init();

        switch(sd_init)
        {
                case 0:
                        UART1_Write_Text("SD FAT Init-OK\r\n");
                break;
                case 1:
                        UART1_Write_Text("SD FAT not found\r\n");
                break;
                default:
                        UART1_Write_Text("SD Init-error\r\n");
                break;
        }
        
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x01);

        while(1)
        {
                SPI_Ethernet_doPacket();
        }
}