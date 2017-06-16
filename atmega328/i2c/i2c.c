char ModuleAddress = 32;

void main()
{
        char value[10];
        TWI_Init(100000);
        UART1_Init(9600);
        UART1_Write_Text("Hello");

        while (1)
        {
                TWI_Start();
                TWI_Write((ModuleAddress<<1)+1);
                ByteToHex(TWI_Read(0),value);
                TWI_Stop();
                UART1_Write_Text(value);
                UART1_Write_Text("\r\nValue: ");
                Delay_ms(1000);
        }
}