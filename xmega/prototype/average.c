#include "average.h"

struct massive ad7705_averaging_massive, measurment_averaging_massive, temperature_averaging_massive;

void Average(void)
{
        increment(&measurment_averaging_massive, oversample(&ad7705_averaging_massive, 32));
        increment(&temperature_averaging_massive, ADCA_Read(3));
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

