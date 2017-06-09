void main()
{
        UART1_Init(9600);
        Delay_ms(100);
        UART1_Write_Text("Hello!\r\n");
        while(1)
        {
                Delay_ms(1000);
                UART1_Write_Text("Hello!\r\n");
        }
}