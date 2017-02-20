sbit Mmc_Chip_Select at PORTC_OUT.B4;
sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

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

void main() {
        int one, two;
        char sd_init = 17, sd_format = 17, sd_exists = 17, sd_assign = 17;
        unsigned long filesize;
        char settings[512] = "one=2\ntwo=1\n";
        unsigned int no_bytes;
        UARTC0_Init(115200);
        Delay_ms(10);
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);
        
        sd_init = Mmc_Fat_Init();
        
        switch(sd_init)
        {
                case 0:
                        UARTC0_Write_Text("SD FAT Init-OK\r\n");
                break;
                case 1:
                        UARTC0_Write_Text("SD FAT not found\r\n");
                break;
                default:
                        UARTC0_Write_Text("SD Init-error\r\n");
                break;
        }
                
        if (sd_init == 1)
        {
                sd_format = Mmc_Fat_QuickFormat("DIGITAL");
                switch(sd_format)
                {
                        case 0:
                                UARTC0_Write_Text("Card was detected, successfully formated and initialized\r\n");
                        break;
                        case 1:
                                UARTC0_Write_Text("FAT16 format was unsuccessful\r\n");
                        break;
                        default:
                                UARTC0_Write_Text("MMC/SD card was not detected\r\n");
                        break;
                }
        }
        
        if (sd_init == 0)
        {
                sd_exists = Mmc_Fat_Exists("SETTINGS.TXT");
                if (sd_exists == 0)
                        UARTC0_Write_Text("file/directory doesn't exist\r\n");
                else if (sd_exists == 1)
                        UARTC0_Write_Text("file/directory exists\r\n");
        }
        
        if (sd_init == 0 || sd_format == 0)
        {
                sd_assign = Mmc_Fat_Assign("SETTINGS.TXT",0x80);
                if(sd_assign == 0)
                        UARTC0_Write_Text("file does not exist and no new file is created\r\n");
                else if (sd_assign == 1)
                        UARTC0_Write_Text("file already exists or file does not exist but a new file is created\r\n");
                else if (sd_assign == 2)
                        UARTC0_Write_Text("there are no more free file handlers, currently opened file is closed in order to free space\r\n");
        }
        
        if (sd_exists == 1)
        {
                Mmc_Fat_Reset(&filesize);
                PrintOut(PrintHandler, "requested file is %d bytes long\r\n",filesize);
                UARTC0_Write_Text(settings);
                no_bytes = Mmc_Fat_ReadN(settings, filesize);
                PrintOut(PrintHandler, "%d bytes is read from file to string\r\n",no_bytes);
                UARTC0_Write_Text(settings);
                if (no_bytes > 0)
                {
                        //one = atoi(getopt(settings, "one"));
                        //two = atoi(getopt(settings, "two"));
                        PrintOut(PrintHandler, "one int value %d\r\n",atoi(getopt(settings,"one")));
                        PrintOut(PrintHandler, "two int value %d\r\n",atoi(getopt(settings,"two")));
                }
                
        }
        else if (sd_assign == 1)
        {
                Mmc_Fat_Rewrite();
                Mmc_Fat_Write("one=1\ntwo=2\n",14);
        }
        
        if (sd_assign == 1)
        {
                Mmc_Fat_Close();
        }
                

        while(1);
}