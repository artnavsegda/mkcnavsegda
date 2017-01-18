#define PORTU1 0x18
#define PORTU2 0x19
#define PORTU3 0x1a

extern unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;
extern unsigned char PORTU1_DIR, PORTU2_DIR, PORTU3_DIR;
extern unsigned char PORTU1_OUT, PORTU2_OUT, PORTU3_OUT;
extern sfr sbit Ignition;
extern sfr sbit IgnitionDirection;

extern sfr sbit CELL_LeftOut;
extern sfr sbit CELL_RightOut;

extern sfr sbit CELL_LeftOut_Direction;
extern sfr sbit CELL_RightOut_Direction;

extern sfr sbit Calibration_Valve;
extern sfr sbit Zero_Valve;

extern sfr sbit Calibration_Valve_Direction;
extern sfr sbit Zero_Valve_Direction;

void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_);
char Expander_Read_Byte(char ModuleAddress, char RegAddress);
void Expander_Set_DirectionPort(char ModuleAddress, char Data_);
void Expander_Write_Port(char ModuleAddress, char Data_);
char Expander_Read_Port(char ModuleAddress);
void Expander_Init(char ModuleAddress);