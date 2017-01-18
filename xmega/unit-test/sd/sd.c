sbit Mmc_Chip_Select at PORTC_OUT.B4;
sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

void readset(void)
{

}

void writeset(void)
{

}

void main() {
        char sd_error;
        UARTC0_Init(115200);
        Delay_ms(10);
        UARTC0_Write_Text("MCU-Started\r\n");
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);
        sd_error = Mmc_Fat_Init();
        if(sd_error == 0)
                UARTC0_Write_Text("SD FAT Init-OK\r\n");
        else if (sd_error == 1)
                UARTC0_Write_Text("SD FAT not found\r\n");
        else
                UARTC0_Write_Text("SD Init-error\r\n");
                
        if (sd_error == 1)
        {
                sd_error = Mmc_Fat_QuickFormat("DIGITAL");
                if(sd_error == 0)
                        UARTC0_Write_Text("Card was detected, successfully formated and initialized\r\n");
                else if (sd_error == 1)
                        UARTC0_Write_Text("FAT16 format was unsuccessful\r\n");
                else
                        UARTC0_Write_Text("MMC/SD card was not detected\r\n");
        }
        
        if (sd_error == 0)
        {
                sd_error = Mmc_Fat_Assign("DIGITAL.TXT",0x80);
                if(sd_error == 0)
                        UARTC0_Write_Text("file does not exist and no new file is created\r\n");
                else if (sd_error == 1)
                        UARTC0_Write_Text("file already exists or file does not exist but a new file is created\r\n");
                else if (sd_error == 2)
                        UARTC0_Write_Text("there are no more free file handlers, currently opened file is closed in order to free space\r\n");
        }
        
        Mmc_Fat_Rewrite();
        Mmc_Fat_Write("Hello world\n",13);
        Mmc_Fat_Close();
                

        while(1);
}