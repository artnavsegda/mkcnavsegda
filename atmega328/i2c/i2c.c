#include "ports.h"

void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_)
{
        TWI_Start();
        TWI_Write(ModuleAddress << 1);
        TWI_Write(RegAddress);
        TWI_Write(Data_);
        TWI_Stop();
}

char Expander_Read_Byte(char ModuleAddress, char RegAddress)
{
        unsigned char value;
        TWI_Start();
        TWI_Write(ModuleAddress << 1);
        TWI_Write(RegAddress);
        TWI_Start();
        TWI_Write((ModuleAddress<<1)+1);
        value = TWI_Read(0);
        TWI_Stop();
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
        TWI_Init(100000);
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}

void main()
{
        /*Expander_Init(PORTU3);
        PORTU3_DIR.B7 = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);

        while(1)
        {
                PORTU3_OUT.B7 = 1;
                Expander_Write_Port(PORTU3,PORTU3_OUT);
                Delay_ms(100);
                PORTU3_OUT.B7 = 0;
                Expander_Write_Port(PORTU3,PORTU3_OUT);
                Delay_ms(100);
        }*/

        Expander_Init(PORTU1);
        CELL_LeftOut_Direction = 0;
        CELL_RightOut_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
        Ignition = 1;
        Expander_Write_Port(PORTU3,PORTU3_OUT);
        //Delay_ms(1000);
        //Ignition = 0;
        //Expander_Write_Port(PORTU3,PORTU3_OUT);

        while (1)
        {
                Delay_ms(1000);
                CELL_LeftOut = 1;
                CELL_RightOut = 0;
             //   Expander_Write_Port(PORTU1,PORTU1_OUT);
                Delay_ms(1000);
                CELL_LeftOut = 0;
                CELL_RightOut = 1;
             //   Expander_Write_Port(PORTU1,PORTU1_OUT);
        }
}