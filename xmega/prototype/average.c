#include "average.h"
#include "adc.h"

struct massive ad7705_averaging_massive, measurment_averaging_massive, temperature_averaging_massive;
extern int zerostage;

void Average(void)
{
        zerostage = oversample(&ad7705_averaging_massive,64)/64;
        increment(&measurment_averaging_massive, zerostage);
        increment(&temperature_averaging_massive, ADCB_Read(ADCB_Cell));
}

long oversample(struct massive *working, unsigned int amount)
{
        return oversample_engine(working->massive,amount,working->position,sizeof(working->massive)/2);
}

long oversample_engine(unsigned int *selekta,int amount, int startpos, int sizeofmassive)
{
        long x = 0;
        int i;
        for(i=0; i<amount; i++)
        {
                if (startpos-i>0)
                x=x+selekta[startpos-i-1];
                else
                x=x+selekta[sizeofmassive+(startpos-i)-1];
        }
        return x;
}

void increment(struct massive *working, unsigned int value)
{
        working->massive[working->position] = value;
                if (working->position++ > sizeof(working->massive)/2)
        working->position = 0;
}