// internal ports
extern sfr sbit LED0;
extern sfr sbit LED0_Direction;
extern sfr sbit LED0_Toggle;
extern sfr sbit LED1;
extern sfr sbit LED1_Direction;
extern sfr sbit LED1_Toggle;
extern sfr sbit LED2;
extern sfr sbit LED3;
extern sfr sbit LED2_Direction;
extern sfr sbit LED3_Direction;
extern sfr sbit LED2_Toggle;
extern sfr sbit LED3_Toggle;

extern sfr sbit SPI_Ethernet_Rst;
extern sfr sbit SPI_Ethernet_CS;
extern sfr sbit SPI_Ethernet_Rst_Direction;
extern sfr sbit SPI_Ethernet_CS_Direction;

extern sfr sbit AD7705_CS;
extern sfr sbit AD7705_CS_Direction;
extern sfr sbit AD7707_DRDY;

// expander ports

#define PORTU1 0x18
#define PORTU2 0x19
#define PORTU3 0x1a

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
extern sfr sbit Calibration_Valve;
extern sfr sbit Zero_Valve;

extern sfr sbit Elemental_Valve_Direction;
extern sfr sbit Calibration_Valve_Direction;
extern sfr sbit Zero_Valve_Direction;

extern sfr sbit CONVERTER;
extern sfr sbit WATLOW1;
extern sfr sbit WATLOW2;
extern sfr sbit WATLOW3;
extern sfr sbit WATLOW4;