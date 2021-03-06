// internal ports
sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_CS_Direction at PORTC_DIR.B4;
sfr sbit AD7707_DRDY at PORTC_IN.B1;

// expander ports
unsigned char PORTU1_OUT = 0x00, PORTU2_OUT = 0x00, PORTU3_OUT = 0x00;

sfr sbit CELL_LeftOut at PORTU1_OUT.B7;
sfr sbit CELL_RightOut at PORTU1_OUT.B6;

sfr sbit Ignition at PORTU3_OUT.B1;

unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;

sfr sbit CELL_LeftIn at PORTU1_OUT.B5;
sfr sbit CELL_RightIn at PORTU1_OUT.B5;

unsigned char PORTU1_DIR = 0xFF, PORTU2_DIR = 0xFF, PORTU3_DIR = 0xFF;

sfr sbit CELL_LeftOut_Direction at PORTU1_DIR.B7;
sfr sbit CELL_RightOut_Direction at PORTU1_DIR.B6;
sfr sbit IgnitionDirection at PORTU3_DIR.B1;
