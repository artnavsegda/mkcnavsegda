#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B1;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;
sfr sbit Mmc_Chip_Select at PORTC_OUT.B4;
sfr sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

char settings[100] = "ip=192.168.1.151 mac=de:ad:be:ef:fe:ed";

char * getopt(char *config2, char *token)
{
        static char config[100];
        char * pch;
        if (strstr(config2,token)==0)
                return "0";
        strcpy(config,config2);
        pch = strtok(strstr(config,token),"=");
        pch = strtok(0," \n");
        return pch;
}

char * getip(char *config2, char *token)
{
        static char ip[4];
        ip[0] = atoi(strtok(getopt(config2,token),"."));
        ip[1] = atoi(strtok(0,"."));
        ip[2] = atoi(strtok(0,"."));
        ip[3] = atoi(strtok(0,"."));
        return ip;
}

char * getmac(char *config2, char *token)
{
        static char mac[6];
        mac[0] = xtoi(strtok(getopt(config2,token),":"));
        mac[1] = xtoi(strtok(0,":"));
        mac[2] = xtoi(strtok(0,":"));
        mac[3] = xtoi(strtok(0,":"));
        mac[4] = xtoi(strtok(0,":"));
        mac[5] = xtoi(strtok(0,":"));
        return mac;
}

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void main()
{
        char sd_init, sd_assign;
        unsigned long filesize;
        //char settings[512];
        UARTC0_Init(115200);//init uart
        Delay_ms(10);//wait for uart to start
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);//init spi
        //PORTC_OUT.B4 = 1;
        sd_init = Mmc_Fat_Init();//init sd card
        switch(sd_init)
        {
                case 0:
                        UARTC0_Write_Text("SD FAT Init-OK\r\n");
                        sd_assign = Mmc_Fat_Assign("SETTINGS.TXT",0);
                        if (sd_assign == 1)
                        {
                                Mmc_Fat_Reset(&filesize);
                                Mmc_Fat_ReadN(settings, filesize);
                        }
                break;
                case 1:
                        UARTC0_Write_Text("SD FAT not found\r\n");
                break;
                default:
                        UARTC0_Write_Text("SD Init-error\r\n");
                break;
        }
        //Mmc_Fat_Assign("DIGITAL.TXT",0x80);
        //Mmc_Fat_Reset(&filesize);
        //Mmc_Fat_ReadN(&settings, filesize);
        //Mmc_Fat_Close();
        SPI_Ethernet_Init(getmac(settings,"mac"), getip(settings,"ip"), 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        while (1)
                SPI_Ethernet_doPacket();
}