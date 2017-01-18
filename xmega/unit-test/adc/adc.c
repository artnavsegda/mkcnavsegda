void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        UARTC0_Init(115200);
        ADCB_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);

        while(1)
        {
                PrintOut(PrintHandler, "ADCB1 %d\r\n", ADCB_Get_Sample(1));
                //PrintOut(PrintHandler, "ADCB1 %d\r\n", ADCB_Read(1));
                delay_ms(100);
        }
}