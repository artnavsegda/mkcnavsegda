#include "i2c.h"

unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;
unsigned char PORTU1_OUT = 0x00, PORTU2_OUT = 0x00, PORTU3_OUT = 0x00;
sfr sbit Ignition at PORTU3_OUT.B1;
unsigned char PORTU1_DIR = 0xFF, PORTU2_DIR = 0xFF, PORTU3_DIR = 0xFF;
sfr sbit IgnitionDirection at PORTU3_DIR.B1;

sfr sbit CELL_LeftOut at PORTU1_OUT.B7;
sfr sbit CELL_RightOut at PORTU1_OUT.B6;

sfr sbit CELL_LeftOut_Direction at PORTU1_DIR.B7;
sfr sbit CELL_RightOut_Direction at PORTU1_DIR.B6;

sfr sbit Elemental_Valve at PORTU3_OUT.B4;
sfr sbit Calibration_Valve at PORTU3_OUT.B3;
sfr sbit Zero_Valve at PORTU3_OUT.B2;

sfr sbit Elemental_Valve_Direction at PORTU3_DIR.B4;
sfr sbit Calibration_Valve_Direction at PORTU3_DIR.B3;
sfr sbit Zero_Valve_Direction at PORTU3_DIR.B2;

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
        TWIE_Init(100000);
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}