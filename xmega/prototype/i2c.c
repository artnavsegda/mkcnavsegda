#include "ports.h"

void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_)
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
}

void main()
{
        TWIE_Init(100000);
        Expander_Init(0x18);
        CELL_LeftOut_Direction = 1;
        CELL_RightOut_Direction = 1;
        Expander_Set_DirectionPort(0x18,PORTU1_DIR);
        Expander_Init(0x19);
        Expander_Set_DirectionPort(0x19,PORTU2_DIR);
        Expander_Init(0x1a);
        IgnitionDirection = 1;
        Expander_Set_DirectionPort(0x1a,PORTU3_DIR);

        while (1)
        {
                Delay_ms(1000);
                CELL_LeftOut = 1;
                CELL_RightOut = 0;
                Expander_Write_Port(0x18,PORTU1_OUT);
                CELL_LeftOut = 0;
                CELL_RightOut = 1;
                Expander_Write_Port(0x18,PORTU1_OUT);
        }
}