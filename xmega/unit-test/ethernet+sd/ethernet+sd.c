#include  <__EthEnc28j60.h>

sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;
sfr sbit Mmc_Chip_Select at PORTE_OUT.B2;
sfr sbit Mmc_Chip_Select_Direction at PORTE_DIR.B2;

char settings[1000] = "ip=192.168.1.151 mac=de:ad:be:ef:fe:ed";
char *options[100];
char *values[100];
int optisize;

char * getmyopt(char *parameter)
{
        int i = 0;
        for (i=0;i<optisize;i++)
                if (strcmp(options[i],parameter)==0)
                        return values[i];
        return 0;
}

char * getmyip(char *token)
{
        static char ip[4];
        char *temp = Malloc(strlen(token));
        strcpy(temp,token);
        ip[0] = atoi(strtok(temp,"."));
        ip[1] = atoi(strtok(0,"."));
        ip[2] = atoi(strtok(0,"."));
        ip[3] = atoi(strtok(0,"."));
        Free(temp,sizeof(temp));
        return ip;
}

char * getmymac(char *token)
{
        static char mac[6];
        char *temp = Malloc(strlen(token));
        strcpy(temp,token);
        mac[0] = xtoi(strtok(temp,":"));
        mac[1] = xtoi(strtok(0,":"));
        mac[2] = xtoi(strtok(0,":"));
        mac[3] = xtoi(strtok(0,":"));
        mac[4] = xtoi(strtok(0,":"));
        mac[5] = xtoi(strtok(0,":"));
        Free(temp,sizeof(temp));
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

void makeopt(void)
{
        int i = 0;
        options[i] = strtok(settings," \r\n");
        while (options[i]!=0)
                options[++i] = strtok(0," \r\n");
        optisize = i;
        for (i=0;i<optisize;i++)
        {
                strtok(options[i],"=");
                values[i] = strtok(0,"=");
        }
}

void main()
{
        char sd_assign, sd_format;
        unsigned long filesize, no_bytes;
        //char settings[512];
        MM_Init();
        UARTC0_Init(115200);//init uart
        Delay_ms(10);//wait for uart to start
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);//init spi
        //PORTC_OUT.B4 = 1;
        switch(Mmc_Fat_Init())
        {
                case 0:
                        UARTC0_Write_Text("SD FAT Init-OK\r\n");
                        if (Mmc_Fat_Assign("SETTINGS.TXT",0) == 1)
                        {
                                Mmc_Fat_Reset(&filesize);
                                no_bytes = Mmc_Fat_ReadN(settings, filesize);
                                Mmc_Fat_Close();
                                settings[no_bytes] = '\0';
                                makeopt();
                        }
                break;
                case 1:
                        UARTC0_Write_Text("SD FAT not found\r\n");
                        sd_format = Mmc_Fat_QuickFormat("DIGITAL");
                break;
                default:
                        UARTC0_Write_Text("SD Init-error\r\n");
                break;
        }
        //Mmc_Fat_Assign("DIGITAL.TXT",0x80);
        //Mmc_Fat_Reset(&filesize);
        //Mmc_Fat_ReadN(&settings, filesize);
        //Mmc_Fat_Close();
        
        UARTC0_Write_Text("expecting ip: ");
        UARTC0_Write_Text(getmyopt("ip"));
        UARTC0_Write_Text("\r\nexpecting mac: ");
        UARTC0_Write_Text(getmyopt("mac"));
        UARTC0_Write_Text(getmyip(getmyopt("ip")));
        
        SPI_Ethernet_Init(getmymac(getmyopt("mac")), getmyip(getmyopt("ip")), 1);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        while (1)
                SPI_Ethernet_doPacket();
}