sbit Mmc_Chip_Select at PORTA_OUT.B1;
sbit Mmc_Chip_Select_Direction at PORTA_DIR.B1;

void main() {
	char sd_error;
	UARTC0_Init(115200);
	Delay_ms(10);
	UARTC0_Write_Text("MCU-Started\n");
	SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV2, _SPI_CLK_LO_LEADING);
	sd_error = Mmc_Init();
	if(sd_error == 0)
	        UARTC0_Write_Text("MMC Init-OK\n");
	else
	        UARTC0_Write_Text("MMC Init-error\n");


	while(1);

	
}