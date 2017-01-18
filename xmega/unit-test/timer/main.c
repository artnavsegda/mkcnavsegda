#include "i2c.h"
#include "timer.h"
#include "ad7705.h"
#include "average.h"

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

#define ADCB_Cell 3

sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

sfr sbit LED2_Direction at PORTD_DIR.B4;
sfr sbit LED2_Toggle at PORTD_OUTTGL.B4;

struct massive firststage;
struct massive secondstage;
struct massive temperature_averaging_massive;

unsigned int timetoexitmode = 0;
unsigned int currentmode = STARTLEVEL;

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

int Modeseconds(enum modelist modeneed)
{
        switch (modeneed)
        {
                case STARTLEVEL: return 10;
                case CELLDELAY: return 10;
                case CELLLEVEL:        return 10;
                case ZERODELAY: return 10;
                case ZEROTEST: return 10;
                case PURGE: return 10;
                case TOTALMERCURYDELAY: return 10;
                case TOTALMERCURY: return 100;
                case ELEMENTALMERCURYDELAY: return 10;
                case ELEMENTALMERCURY: return 10;
                case PRECALIBRATIONDELAY: return 10;
                case CALIBRATION: return 10;
                case POSTCALIBRATIONDELAY: return 10;
        }
        return 0;
}

void Entermode(enum modelist modetoenter)
{
        currentmode = modetoenter;
        timetoexitmode = Modeseconds(modetoenter);
        switch(modetoenter)
        {
                case STARTLEVEL:
                        Ignition = 1;
                break;
                case CELLDELAY:
                        CELL_LeftOut = 1;
                        CELL_RightOut = 0;
                break;
                case CELLLEVEL:
                break;
                case ZERODELAY:
                        Zero_Valve = 1;
                        CELL_LeftOut = 0;
                        CELL_RightOut = 1;
                return;
                break;
                case ZEROTEST:
                break;
                case PURGE:
                break;
                case TOTALMERCURYDELAY:
                break;
                case TOTALMERCURY:
                break;
                case ELEMENTALMERCURYDELAY:
                break;
                case ELEMENTALMERCURY:
                break;
                case PRECALIBRATIONDELAY:
                        Calibration_Valve = 1;
                break;
                case CALIBRATION:
                break;
                case POSTCALIBRATIONDELAY:
                break;
                default:
                break;
        }
}

enum modelist Sequence(enum modelist modetosequence)
{
        switch(modetosequence)
        {
                case STARTLEVEL: return ZERODELAY;
                case CELLDELAY: return CELLLEVEL;
                case CELLLEVEL:        return TOTALMERCURYDELAY;
                case ZERODELAY:        return ZEROTEST;
                case ZEROTEST: return CELLDELAY;
                case PURGE:        return PURGE;
                case TOTALMERCURYDELAY:        return TOTALMERCURY;
                case TOTALMERCURY: return ZERODELAY;
                case ELEMENTALMERCURYDELAY:        return ELEMENTALMERCURY;
                case ELEMENTALMERCURY: return TOTALMERCURYDELAY;
                case PRECALIBRATIONDELAY: return CALIBRATION;
                case CALIBRATION: return POSTCALIBRATIONDELAY;
                case POSTCALIBRATIONDELAY: return PRECALIBRATIONDELAY;
        }
        return modetosequence;
}

unsigned int coefficent = 0x17CC;
unsigned int zerolevelavg = 0x17CC;
unsigned int celllevelavg = 0x17CC;
unsigned int celltempavg = 0;

void Exitmode(enum modelist modetoexit)
{
        switch(modetoexit)
        {
                case STARTLEVEL:
                        Ignition = 0;
                break;
                case CELLDELAY:
                break;
                case CELLLEVEL:
                        celllevelavg = oversample(&secondstage,modeseconds(CELLLEVEL))/modeseconds(CELLLEVEL);
                        celltempavg = oversample(&temperature_averaging_massive,modeseconds(CELLLEVEL))/modeseconds(CELLLEVEL);
                        CELL_LeftOut = 0;
                        CELL_RightOut = 1;
                break;
                case ZERODELAY:
                break;
                case ZEROTEST:
                        zerolevelavg = oversample(&secondstage,modeseconds(ZEROTEST))/modeseconds(ZEROTEST);
                        Zero_Valve = 0;
                break;
                case PURGE:
                break;
                case TOTALMERCURYDELAY:
                break;
                case TOTALMERCURY:
                break;
                case ELEMENTALMERCURYDELAY:
                break;
                case ELEMENTALMERCURY:
                break;
                case PRECALIBRATIONDELAY:
                break;
                case CALIBRATION:
                        coefficent = oversample(&secondstage,modeseconds(CALIBRATION))/modeseconds(CALIBRATION);
                        Calibration_Valve = 0;
                break;
                case POSTCALIBRATIONDELAY:
                break;
                default:
                break;
        }
        Entermode(Sequence(modetoexit));
}

int tick = 0;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        tick = 1;
}

void Ports_Init(void)
{
        LED0_Direction = 1;
        LED2_Direction = 1;
        Expander_Init(PORTU1);
        CELL_LeftOut_Direction = 0;
        CELL_RightOut_Direction = 0;
        Calibration_Valve_Direction = 0;
        Zero_Valve_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU2);
        Expander_Set_DirectionPort(PORTU2,PORTU2_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

unsigned int result;

void Print_Info(void)
{
        PrintOut(PrintHandler, "======= frame =======\r\n");
        PrintOut(PrintHandler, "mode: %d\r\n", currentmode);
        PrintOut(PrintHandler, "run %d\r\n", Modeseconds(currentmode));
        PrintOut(PrintHandler, "countdown: %d\r\n", timetoexitmode);
        PrintOut(PrintHandler, "next: %d\r\n", (int)Sequence(currentmode));
        PrintOut(PrintHandler, "next run: %d\r\n", Modeseconds(Sequence(currentmode)));
        PrintOut(PrintHandler, "DATA(r): %5d\r\n", BSWAP_16(result)-0x17CC);
        PrintOut(PrintHandler, "DATA(x16): %5d\r\n", (oversample(&firststage,64)/64)-0x17CC);
        PrintOut(PrintHandler, "TEMP(r): %5d\r\n", ADCB_Read(ADCB_Cell));
        PrintOut(PrintHandler, "======= static =======\r\n");
        PrintOut(PrintHandler, "CFC(r): %5d\r\n", coefficent-0x17CC);
        PrintOut(PrintHandler, "ZLA(r): %5d\r\n", zerolevelavg-0x17CC);
        PrintOut(PrintHandler, "CLA(r): %5d\r\n", celllevelavg-0x17CC);
        PrintOut(PrintHandler, "CTA(r): %d\r\n", celltempavg);
        PrintOut(PrintHandler, "======= IO =======\r\n");
        PrintOut(PrintHandler, "U1_IN: %x\r\n", (int)PORTU1_IN);
        PrintOut(PrintHandler, "U1_OUT: %x\r\n", (int)PORTU1_OUT);
        PrintOut(PrintHandler, "U2_IN: %x\r\n", (int)PORTU2_IN);
        PrintOut(PrintHandler, "U2_OUT: %x\r\n", (int)PORTU2_OUT);
        PrintOut(PrintHandler, "U3_IN: %x\r\n", (int)PORTU3_IN);
        PrintOut(PrintHandler, "U3_OUT: %x\r\n", (int)PORTU3_OUT);
        PrintOut(PrintHandler, "======= end frame =======\r\n");
}

void main()
{
        Ports_Init();
        UARTC0_Init(115200);
        AD7705_Init();
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        PrintOut(PrintHandler, "START\r\n");
        Entermode(STARTLEVEL);
        
        while (1)
        {
                if (AD7705_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                        increment(&firststage,BSWAP_16(result));
                }
                if (tick == 1)
                {
                        tick = 0;
                        LED2_Toggle = 1;
                        increment(&secondstage,oversample(&firststage,64)/64);
                        increment(&temperature_averaging_massive,ADCB_Read(ADCB_Cell));
                        timetoexitmode--;
                        if (timetoexitmode == 0)
                                Exitmode(currentmode);

                        PORTU1_IN = Expander_Read_Port(PORTU1);
                        PORTU2_IN = Expander_Read_Port(PORTU2);
                        PORTU3_IN = Expander_Read_Port(PORTU3);
                        Print_Info();
                        Expander_Write_Port(PORTU1,PORTU1_OUT);
                        Expander_Write_Port(PORTU2,PORTU2_OUT);
                        Expander_Write_Port(PORTU3,PORTU3_OUT);
                }
        }
}