sbit Mmc_Chip_Select at PORTC_OUT.B4;
sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

int getvalue(char value[], char buffer[])
{
        char * pch;
        pch = strtok(strstr(buffer,value),"=");
        pch = strtok(0," \n");
        return atoi(pch);
}

void writeset(void)
{

}

void main() {
	int one, two;
        char sd_init, sd_format, sd_exists, sd_assign;
        unsigned long filesize;
        char settings[512];
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
                if(sd_format == 0)
                        UARTC0_Write_Text("Card was detected, successfully formated and initialized\r\n");
                else if (sd_format == 1)
                        UARTC0_Write_Text("FAT16 format was unsuccessful\r\n");
                else
                        UARTC0_Write_Text("MMC/SD card was not detected\r\n");
        }
        
        if (sd_init == 0)
        {
                sd_exists = Mmc_Fat_Exists("DIGITAL.TXT");
                if (sd_exists == 0)
                        UARTC0_Write_Text("file/directory doesn't exist\r\n");
                else if (sd_exists == 1)
                        UARTC0_Write_Text("file/directory exists\r\n");
        }
        
        if (sd_init == 0 || sd_format == 0)
        {
                sd_assign = Mmc_Fat_Assign("DIGITAL.TXT",0x80);
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
                no_bytes = Mmc_Fat_ReadN(&settings, filesize);
                if (no_bytes > 0)
                {
                        one = getvalue(settings, "one");
                        two = getvalue(settings, "two");
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