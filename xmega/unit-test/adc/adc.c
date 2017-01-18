void PrintHandler(char c)
{
        UARTC0_Write(c);
}

#define ADCB_Cell 3

float ADC_Voltage(unsigned int adcvalue);
float TMP_Celsius(float voltage);

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

void main() {
        UARTC0_Init(115200);
        ADCB_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);

        while(1)
        {
        	PrintOut(PrintHandler, "===================== START ==================\r\n");
                PrintOut(PrintHandler, "TEMP(r): %5d\r\n", ADCB_Get_Sample(ADCB_Cell));
                PrintOut(PrintHandler, "TEMP(V): %5f\r\n", (ADCB_Get_Sample(ADCB_Cell)-180)*((3.3/1.6)/4095));
                PrintOut(PrintHandler, "TEMP(C): %5f\r\n", (((ADCB_Get_Sample(ADCB_Cell)-180)*((3.3/1.6)/4095))-0.5)*100);
                delay_ms(1000);
        }
}