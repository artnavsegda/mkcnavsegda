void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_);
char Expander_Read_Byte(char ModuleAddress, char RegAddress);
void Expander_Set_DirectionPort(char ModuleAddress, char Data_);
void Expander_Write_Port(char ModuleAddress, char Data_);
char Expander_Read_Port(char ModuleAddress);
void Expander_Init(char ModuleAddress);
void Expander_Read_All(void);
void Expander_Write_All(void);
