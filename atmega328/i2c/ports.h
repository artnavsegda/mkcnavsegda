#define PORTU1 0x18
#define PORTU2 0x19
#define PORTU3 0x1a

extern sfr sbit CELL_LeftOut_Direction;
extern sfr sbit CELL_RightOut_Direction;
extern sfr sbit IgnitionDirection;
extern unsigned char PORTU1_DIR, PORTU2_DIR, PORTU3_DIR;
extern sfr sbit CELL_LeftOut;
extern sfr sbit CELL_RightOut;
extern unsigned char PORTU1_OUT, PORTU2_OUT, PORTU3_OUT;
extern sfr sbit Ignition;