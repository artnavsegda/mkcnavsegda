sbit Mmc_Chip_Select at PORTD4_bit;
sbit Mmc_Chip_Select_Direction at DDD4_bit;

void main()
{
        char sd_error;
        UART1_Init(115200);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        SPI1_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);
        sd_error = Mmc_Init();
        if(sd_error == 0)
                UART1_Write_Text("MMC Init-OK\r\n");
        else
                UART1_Write_Text("MMC Init-error\r\n");

        while(1);
}
