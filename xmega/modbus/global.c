#include "adc.h"
#include "ad7705.h"
#include "average.h"

struct massive burnstage;
struct massive firststage;
struct massive secondstage;
struct massive thirdstage;
struct massive temperature_averaging_massive;

unsigned int result;
int zerostage, pseudostage;
int coefficent = ADCZERO;
int zerolevelavg = ADCZERO;
int celllevelavg = 4000+ADCZERO;
int celltempavg = 1670;
int tick = 0;
char sd_init = 9;
char webpage[1410];
unsigned int len = 0;