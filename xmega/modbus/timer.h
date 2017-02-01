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

extern unsigned int timetoexitmode;
extern enum modelist currentmode;

unsigned int Modeseconds(enum modelist modeneed);
void Entermode(enum modelist modetoenter);
enum modelist Sequence(enum modelist modetosequence);
void Exitmode(enum modelist modetoexit);
void Operatemode(void);