#include "timer.h"
#include "i2c.h"
#include "average.h"
#include "modbus.h"

unsigned int timetoexitmode = 0;
unsigned char currentmode = STARTLEVEL;

extern unsigned int coefficent;
extern unsigned int zerolevelavg;
extern unsigned int celllevelavg;
extern unsigned int celltempavg;

extern struct massive secondstage;
extern struct massive temperature_averaging_massive;

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
                        CELL_LeftOut = 1;
                        CELL_RightOut = 0;
                break;
                case CELLDELAY:
                        CELL_LeftOut = 0;
                        CELL_RightOut = 1;
                break;
                case CELLLEVEL:
                break;
                case ZERODELAY:
                        bctable[3] = 1;
                        Zero_Valve = 1;
                return;
                break;
                case ZEROTEST:
                break;
                case PURGE:
                break;
                case TOTALMERCURYDELAY:
                break;
                case TOTALMERCURY:
                        bctable[2] = 1;
                break;
                case ELEMENTALMERCURYDELAY:
                break;
                case ELEMENTALMERCURY:
                break;
                case PRECALIBRATIONDELAY:
                        bctable[4] = 1;
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
                        celllevelavg = oversample(&secondstage,Modeseconds(CELLLEVEL))/Modeseconds(CELLLEVEL);
                        celltempavg = oversample(&temperature_averaging_massive,Modeseconds(CELLLEVEL))/Modeseconds(CELLLEVEL);
                        CELL_LeftOut = 1;
                        CELL_RightOut = 0;
                break;
                case ZERODELAY:
                break;
                case ZEROTEST:
                        zerolevelavg = oversample(&secondstage,Modeseconds(ZEROTEST))/Modeseconds(ZEROTEST);
                        Zero_Valve = 0;
                        bctable[3] = 0;
                break;
                case PURGE:
                break;
                case TOTALMERCURYDELAY:
                break;
                case TOTALMERCURY:
                        bctable[2] = 0;
                break;
                case ELEMENTALMERCURYDELAY:
                break;
                case ELEMENTALMERCURY:
                break;
                case PRECALIBRATIONDELAY:
                break;
                case CALIBRATION:
                        coefficent = oversample(&secondstage,Modeseconds(CALIBRATION))/Modeseconds(CALIBRATION);
                        Calibration_Valve = 0;
                break;
                case POSTCALIBRATIONDELAY:
                        bctable[4] = 0;
                break;
                default:
                break;
        }
        Entermode(Sequence(modetoexit));
}

void Operatemode(void)
{
        if (currentmode == TOTALMERCURY || currentmode == PURGE)
        {
                if (bctable[99] == 1)
                {
                        bctable[99] == 0;
                        Entermode(PRECALIBRATIONDELAY);
                }
                if (bctable[100] == 1)
                {
                        bctable[100] == 0;
                        Entermode(ZERODELAY);
                }
                if (bctable[101] == 1)
                {
                        bctable[101] == 0;
                        Entermode(ELEMENTALMERCURYDELAY);
                }
                if (bctable[102] == 1)
                {
                        bctable[102] == 0;
                        Entermode(PURGE);
                }
                if (bctable[103] == 1)
                {
                        bctable[103] == 0;
                        Exitmode(PURGE);
                }
        }
}