sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;
sfr sbit Mmc_Chip_Select at PORTC_OUT.B4;
sfr sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

char * getvalue(char *value, char *buffer2)
{
        char * pch;
        static char buffer[100];
        strcpy(buffer,buffer2);
        pch = strtok(strstr(buffer,value),"=");
        pch = strtok(0," \n");
        return pch;
}

char * getmac(char *value, char buffer2)
{
        static char mac[6];
        mac[0] = xtoi(strtok(getvalue(value,buffer2),":"));
        mac[1] = xtoi(strtok(0,":"));
        mac[2] = xtoi(strtok(0,":"));
        mac[3] = xtoi(strtok(0,":"));
        mac[4] = xtoi(strtok(0,":"));
        mac[5] = xtoi(strtok(0,":"));
        return mac;
}

char * getip(char *value, char buffer2)
{
        static char ip[4];
        ip[0] = atoi(strtok(getvalue(value,buffer2),"."));
        ip[1] = atoi(strtok(0,"."));
        ip[2] = atoi(strtok(0,"."));
        ip[3] = atoi(strtok(0,"."));
        return ip;
}

void main()
{
        unsigned long filesize;
        char settings[512];
        UARTC0_Init(115200);
        Delay_ms(10);
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);
        PORTC_OUT.B4 = 1;
        Mmc_Fat_Init();
        Mmc_Fat_Assign("DIGITAL.TXT",0x80);
        Mmc_Fat_Reset(&filesize);
        Mmc_Fat_ReadN(&settings, filesize);
        Mmc_Fat_Close();
        SPI_Ethernet_Init(getmac("mac",settings),getip("ip",settings),1);
}