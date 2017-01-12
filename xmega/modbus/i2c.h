extern unsigned char PORTU1_DIR, PORTU2_DIR, PORTU3_DIR;
extern unsigned char PORTU1_OUT, PORTU2_OUT, PORTU3_OUT;
extern unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;

void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_);
char Expander_Read_Byte(char ModuleAddress, char RegAddress);
void Expander_Set_DirectionPort(char ModuleAddress, char Data_);
void Expander_Write_Port(char ModuleAddress, char Data_);
char Expander_Read_Port(char ModuleAddress);
void Expander_Init(char ModuleAddress);