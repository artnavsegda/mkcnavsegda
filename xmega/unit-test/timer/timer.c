#include "i2c.h"

sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

enum modelist {
        STARTLEVEL = 5,
        CELLDELAY = 7,
        CELLLEVEL = 8,
        ZERODELAY = 11,
        ZEROTEST = 12,
        PURGE = 13,
        TOTALMERCURYDELAY = 21,
        TOTALMERCURY = 22,
        ELEMENTALMERCURYDELAY = 26,
        ELEMENTALMERCURY = 27,
        PRECALIBRATIONDELAY = 31,
        CALIBRATION = 32,
        POSTCALIBRATIONDELAY = 33
};

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
                case STARTLEVEL: return CELLDELAY;
                case CELLDELAY: return CELLDELAY;
                case CELLLEVEL:        return CELLDELAY;
                case ZERODELAY:        return CELLDELAY;
                case ZEROTEST: return CELLDELAY;
                case PURGE:        return CELLDELAY;
                case TOTALMERCURYDELAY:        return CELLDELAY;
                case TOTALMERCURY: return CELLDELAY;
                case ELEMENTALMERCURYDELAY:        return CELLDELAY;
                case ELEMENTALMERCURY: return CELLDELAY;
                case PRECALIBRATIONDELAY: return CELLDELAY;
                case CALIBRATION: return CELLDELAY;
                case POSTCALIBRATIONDELAY: return CELLDELAY;
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

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        LED0_Toggle = 1;
        timetoexitmode--;
        if (timetoexitmode == 0)
                Exitmode(currentmode);
        Expander_Write_Port(PORTU3,PORTU3_OUT);
}

void main()
{
        LED0_Direction = 1;
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        PORTU3_OUT = 0x00;
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        UARTC0_Init(115200);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,0x00);
        Entermode(STARTLEVEL);
        
        while(1);
}