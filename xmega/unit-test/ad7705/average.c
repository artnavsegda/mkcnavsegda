#include "average.h"

long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive) // ??????????
{
        int i;
        long x = 0;
        for(i=0; i<amount; i++)
        {
                if (startpos-i>0)
			x=x+selekta[startpos-i-1];
                else
			x=x+selekta[sizeofmassive+(startpos-i)-1];
        }
        return x;
}

long oversample(struct massive *working, unsigned int amount)
{
        return average(working->massive,amount,working->position,sizeof(working->massive)/2);
}

void increment(struct massive *working, unsigned int value)
{
        working->massive[working->position] = value;
                if (working->position++ > sizeof(working->massive)/2)
        working->position = 0;
}
