sbit Mmc_Chip_Select           at LATB0_bit;  // for writing to output pin always use latch (PIC18 family)
sbit Mmc_Chip_Select_Direction at TRISB0_bit;

void main()
{
        char sd_init, sd_format, sd_exists, sd_assign;
        UART1_Init(115200);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        TRISD3_bit = 0; //set ethernet cs as output
        SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
        LATD3_bit = 1; //shut up ethernet controller
        
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
        
        while (1)
                ;
}