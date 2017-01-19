float ADC_Voltage(unsigned int adcvalue)
{
        const float popugai = (3.283/1.603)/4095;
        const int adczero = 186;
        return (adcvalue-adczero)*popugai;
}

float TMP_Celsius(float voltage)
{
        return ((voltage-0.5)*100);
}