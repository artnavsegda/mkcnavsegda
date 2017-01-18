#include "adc.h"

float ADC_Voltage(unsigned int adcvalue)
{
        const float popugai = (3.3/1.6)/4095;
        const int adczero = 180;
        return (adcvalue-adczero)*popugai;
}

float TMP_Celsius(float voltage)
{
	return ((voltage-0.5)*100);
}


