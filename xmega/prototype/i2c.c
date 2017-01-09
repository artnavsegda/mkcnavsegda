/*void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_)
{
        TWIE_Start();
        TWIE_Write(ModuleAddress << 1);
        TWIE_Write(RegAddress);
        TWIE_Write(Data_);
        TWIE_Stop();
}

char Expander_Read_Byte(char ModuleAddress, char RegAddress)
{
        unsigned char value;
        TWIE_Start();
        TWIE_Write(ModuleAddress << 1);
        TWIE_Write(RegAddress);
        TWIE_Start();
        TWIE_Write((ModuleAddress<<1)+1);
        value = TWIE_Read(0);
        TWIE_Stop();
        return value;
}

void Expander_Set_DirectionPort(char ModuleAddress, char Data_)
{
        Expander_Write_Byte(ModuleAddress, 0x03, Data_);
}

void Expander_Write_Port(char ModuleAddress, char Data_)
{
        Expander_Write_Byte(ModuleAddress, 0x01, Data_);
}

char Expander_Read_Port(char ModuleAddress)
{
        return Expander_Read_Byte(ModuleAddress, 0x00);
}

void Expander_Init(char ModuleAddress)
{
        TWIE_Init(100000);
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}*/