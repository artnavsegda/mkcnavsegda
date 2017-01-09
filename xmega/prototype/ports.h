// internal ports
extern sfr sbit SPI_Ethernet_Rst;
extern sfr sbit SPI_Ethernet_CS;
extern sfr sbit SPI_Ethernet_Rst_Direction;
extern sfr sbit SPI_Ethernet_CS_Direction;

extern sfr sbit AD7705_CS;
extern sfr sbit AD7705_CS_Direction;
extern sfr sbit AD7707_DRDY;

// expander ports
extern unsigned char PORTU1_OUT, PORTU2_OUT, PORTU3_OUT;

extern sfr sbit CELL_LeftOut;
extern sfr sbit CELL_RightOut;

extern sfr sbit Ignition;

extern unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;

extern sfr sbit CELL_LeftIn;
extern sfr sbit CELL_RightIn;

extern unsigned char PORTU1_DIR, PORTU2_DIR, PORTU3_DIR;

extern sfr sbit CELL_LeftOut_Direction;
extern sfr sbit CELL_RightOut_Direction;

extern sfr sbit IgnitionDirection;