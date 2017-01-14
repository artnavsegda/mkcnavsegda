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
unsigned char currentmode = STARTLEVEL;

int Modeseconds(enum modelist modeneed)
{
        switch (modeneed)
        {
                case STARTLEVEL: return 10;
                case CELLDELAY: return 100;
                case CELLLEVEL:        return 100;
                case ZERODELAY: return 100;
                case ZEROTEST: return 100;
                case PURGE: return 100;
                case TOTALMERCURYDELAY: return 100;
                case TOTALMERCURY: return 100;
                case ELEMENTALMERCURYDELAY: return 100;
                case ELEMENTALMERCURY: return 100;
                case PRECALIBRATIONDELAY: return 100;
                case CALIBRATION: return 100;
                case POSTCALIBRATIONDELAY: return 100;
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
                        //Expander_Write_Port(PORTU3,PORTU3_OUT);
                break;
                case CELLDELAY:
                break;
                case CELLLEVEL:
                break;
                case ZERODELAY:
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
                break;
                case ZERODELAY:
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
                break;
                case CALIBRATION:
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
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU2);
        Expander_Set_DirectionPort(PORTU2,PORTU2_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void main()
{
        unsigned int result;
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        Ports_Init();
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        UARTC0_Init(115200);
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
                        LED0_Toggle = 1;
                        increment(&secondstage,oversample(&firststage,64)/64);
                        increment(&temperature_averaging_massive,ADCB_Read(ADCB_Cell));
                        timetoexitmode--;
                        if (timetoexitmode == 0)
                                Exitmode(currentmode);
                        Expander_Write_Port(PORTU1,PORTU1_OUT);
                        Expander_Write_Port(PORTU2,PORTU2_OUT);
                        Expander_Write_Port(PORTU3,PORTU3_OUT);
                }
        }
}