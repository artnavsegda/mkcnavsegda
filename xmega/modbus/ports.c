sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;

sfr sbit Mmc_Chip_Select at PORTE_OUT.B2;
sfr sbit Mmc_Chip_Select_Direction at PORTE_DIR.B2;

sfr sbit AD7707_DRDY at PORTC_IN.B1;
sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit LED0 at PORTR_OUT.B0;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;
sfr sbit LED1 at PORTR_OUT.B1;
sfr sbit LED1_Direction at PORTR_DIR.B1;
sfr sbit LED1_Toggle at PORTR_OUTTGL.B1;
sfr sbit LED2 at PORTD_OUT.B4;
sfr sbit LED3 at PORTD_OUT.B5;
sfr sbit LED2_Direction at PORTD_DIR.B4;
sfr sbit LED3_Direction at PORTD_DIR.B5;
sfr sbit LED2_Toggle at PORTD_OUTTGL.B4;
sfr sbit LED3_Toggle at PORTD_OUTTGL.B5;