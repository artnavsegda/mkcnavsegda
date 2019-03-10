sbit Mmc_Chip_Select at PORTC_OUT.B4;
sbit Mmc_Chip_Select_Direction at PORTC_DIR.B4;

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        int one, two;
        char sd_init = 17, sd_format = 17;
        UARTC0_Init(9600);
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

        while(1);
}