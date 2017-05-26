sbit Mmc_Chip_Select           at LATD3_bit;  // for writing to output pin always use latch (PIC18 family)
sbit Mmc_Chip_Select_Direction at TRISD3_bit;

void main()
{
        char sd_init, sd_format, sd_exists, sd_assign;
        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
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
        
        if (sd_init == 1)
        {
                sd_format = Mmc_Fat_QuickFormat("DIGITAL");
                switch(sd_format)
                {
                        case 0:
                                UART1_Write_Text("Card was detected, successfully formated and initialized\r\n");
                        break;
                        case 1:
                                UART1_Write_Text("FAT16 format was unsuccessful\r\n");
                        break;
                        default:
                                UART1_Write_Text("MMC/SD card was not detected\r\n");
                        break;
                }
        }
        
        while (1)
                ;
}