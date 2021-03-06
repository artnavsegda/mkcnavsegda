#include "average.h"
#include "log.h"

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

int minmax2(unsigned int *selekta,int amount, int startpos, int sizeofmassive) // ??????????
{
        int i, x;
        int xmin = 32000, xmax = 0;
        for(i=0; i<amount; i++)
        {
                if (startpos-i>0)
                        x = selekta[startpos-i-1];
                else
                        x = selekta[sizeofmassive+(startpos-i)-1];
                if (x < xmin)
                        xmin = x;
                if (x > xmax)
                        xmax = x;
        }
        return xmax-xmin;
}

unsigned int leverage(unsigned int *selekta,int i, int startpos, int sizeofmassive)
{
        int x = 0;
        if (startpos-i>0)
                 x=selekta[startpos-i-1];
        else
                x=selekta[sizeofmassive+(startpos-i)-1];
        return x;
}

unsigned int wayback(struct massive *working, unsigned int backstep)
{
        return leverage(working->massive,backstep,working->position,sizeof(working->massive)/2);
}

long oversample(struct massive *working, unsigned int amount)
{
        return average(working->massive,amount,working->position,sizeof(working->massive)/2);
}

int minmax1(struct massive *working, unsigned int amount)
{
        return minmax2(working->massive,amount,working->position,sizeof(working->massive)/2);
}

void increment(struct massive *working, unsigned int value)
{
        working->massive[working->position] = value;
                if (working->position++ > sizeof(working->massive)/2)
        working->position = 0;
}