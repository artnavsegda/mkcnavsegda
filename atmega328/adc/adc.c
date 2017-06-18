void main()
{
	char string[10];
        UART1_Init(9600);
        Delay_ms(100);
        UART1_Write_Text("ADC read:\r\n");
        while(1)
        {
                Delay_ms(1000);
                IntToHex(ADC_Read(0),string);
                UART1_Write_Text(string);
                UART1_Write_Text("\r\n");
        }
}
