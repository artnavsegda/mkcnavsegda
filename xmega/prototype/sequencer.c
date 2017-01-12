#include "sequencer.h"
#include "average.h"
#include "ports.h"

unsigned int timetoexitmode = 0;
unsigned char currentmode = STARTLEVEL;

extern int coefficent;
extern int zerolevelavg;
int celllevelavg;
int celltempavg;

void Sequencer(void)
{
        timetoexitmode--;
        if (timetoexitmode == 0)
                Exitmode(currentmode);
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
                        celllevelavg = oversample(&measurment_averaging_massive,modeseconds(CELLLEVEL))/modeseconds(CELLLEVEL);
                        celltempavg = oversample(&temperature_averaging_massive,modeseconds(CELLLEVEL))/modeseconds(CELLLEVEL);
                        CELL_LeftOut = 0;
                        CELL_RightOut = 1;
                break;
                case ZERODELAY:
                break;
                case ZEROTEST:
                        zerolevelavg = oversample(&measurment_averaging_massive,modeseconds(ZEROTEST))/modeseconds(ZEROTEST);
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
                        coefficent = oversample(&measurment_averaging_massive,modeseconds(CALIBRATION))/modeseconds(CALIBRATION);
                        Calibration_Valve = 0;
                break;
                case POSTCALIBRATIONDELAY:
                break;
                default:
                break;
        }
        Entermode(Sequence(modetoexit));
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

int Modeseconds(enum modelist modeneed)
{
        switch (modeneed)
        {
                case STARTLEVEL: return 100;
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