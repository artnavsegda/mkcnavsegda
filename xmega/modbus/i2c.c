#include "i2c.h"

unsigned char PORTU1_OUT = 0x00, PORTU2_OUT = 0x00, PORTU3_OUT = 0x00;
unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;
unsigned char PORTU1_DIR = 0xFF, PORTU2_DIR = 0xFF, PORTU3_DIR = 0xFF;

sfr sbit CELL_LeftOut at PORTU1_OUT.B7;
sfr sbit CELL_RightOut at PORTU1_OUT.B6;
sfr sbit CELL_LeftOut_Direction at PORTU1_DIR.B7;
sfr sbit CELL_RightOut_Direction at PORTU1_DIR.B6;
sfr sbit Ignition at PORTU3_OUT.B1;
sfr sbit IgnitionDirection at PORTU3_DIR.B1;

sfr sbit Calibration_Valve at PORTU3_OUT.B3;
sfr sbit Zero_Valve at PORTU3_OUT.B2;
sfr sbit Elemental_Valve at PORTU3_OUT.B4;
sfr sbit Purge_Valve at PORTU3_OUT.B5;

sfr sbit Calibration_Valve_Direction at PORTU3_DIR.B3;
sfr sbit Zero_Valve_Direction at PORTU3_DIR.B2;
sfr sbit Elemental_Valve_Direction at PORTU3_DIR.B3;
sfr sbit Purge_Valve_Direction at PORTU3_DIR.B2;

sfr sbit SERVO_4_RIGHT_IN at PORTU3_IN.B6;
sfr sbit SERVO_2_RIGHT_IN at PORTU2_IN.B7;
sfr sbit SERVO_2_LEFT_IN at PORTU1_IN.B1;
sfr sbit SERVO_3_RIGHT_IN at PORTU2_IN.B3;
sfr sbit SERVO_3_LEFT_IN at PORTU2_IN.B4;

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
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}

void Expander_Read_All(void)
{
        PORTU1_IN = Expander_Read_Port(PORTU1);
        PORTU2_IN = Expander_Read_Port(PORTU2);
        PORTU3_IN = Expander_Read_Port(PORTU3);
}

void Expander_Write_All(void)
{
        Expander_Write_Port(PORTU1,PORTU1_OUT);
        Expander_Write_Port(PORTU2,PORTU2_OUT);
        Expander_Write_Port(PORTU3,PORTU3_OUT);
}

void Expander_Direction_All(void)
{
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Set_DirectionPort(PORTU2,PORTU2_DIR);
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void Expander_Init_All(void)
{
        Expander_Init(PORTU1);
        Expander_Init(PORTU2);
        Expander_Init(PORTU3);
}