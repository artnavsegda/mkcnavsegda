#include "average.h"

long average(int *selekta,int amount, int startpos, int sizeofmassive) // ??????????
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

long oversample(struct massive *working, int amount)
{
        return average(working->massive,amount,working->position,MASSIVE_SIZE);
}

void increment(struct massive *working, int value)
{
        working->massive[working->position] = value;
                if (working->position++ > MASSIVE_SIZE)
        working->position = 0;
}