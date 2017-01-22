// internal ports
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

sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_CS_Direction at PORTC_DIR.B4;
sfr sbit AD7707_DRDY at PORTC_IN.B1;

// expander ports
unsigned char PORTU1_OUT, PORTU2_OUT, PORTU3_OUT;
unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;
unsigned char PORTU1_DIR, PORTU2_DIR, PORTU3_DIR;

sfr sbit CELL_LeftOut at PORTU1_OUT.B7;
sfr sbit CELL_LeftOut_Direction at PORTU1_DIR.B7;
sfr sbit CELL_RightOut at PORTU1_OUT.B6;
sfr sbit CELL_RightOut_Direction at PORTU1_DIR.B6;
sfr sbit CELL_LeftIn at PORTU1_OUT.B5;
sfr sbit CELL_RightIn at PORTU1_OUT.B5;

sfr sbit Ignition at PORTU3_OUT.B1;
sfr sbit IgnitionDirection at PORTU3_DIR.B1;

sfr sbit Elemental_Valve at PORTU3_OUT.B4;
sfr sbit Elemental_Valve_Direction at PORTU3_DIR.B4;
sfr sbit Calibration_Valve at PORTU3_OUT.B3;
sfr sbit Calibration_Valve_Direction at PORTU3_DIR.B3;
sfr sbit Zero_Valve at PORTU3_OUT.B2;
sfr sbit Zero_Valve_Direction at PORTU3_DIR.B2;

sfr sbit CONVERTER at PORTU3_IN.B6;
sfr sbit WATLOW1 at PORTU2_IN.B7;
sfr sbit WATLOW2 at PORTU1_IN.B1;
sfr sbit WATLOW3 at PORTU2_IN.B3;
sfr sbit WATLOW4 at PORTU2_IN.B4;
