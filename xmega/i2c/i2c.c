char ModuleAddress = 0x18;

void main()
{
        char value[10];
        TWIE_Init(100000);
        UARTC0_Init(9600);
        UARTC0_Write_Text("Hello\r\n");

        while (1)
        {
                TWI_Start();
                TWIE_Write(ModuleAddress<<1);
                TWIE_Write(0x00);
                TWIE_Start();
                TWIE_Write((ModuleAddress<<1)+1);
                ByteToHex(TWIE_Read(0),value);
                TWIE_Stop();
                UARTC0_Write_Text(value);
                UARTC0_Write_Text("\r\nValue: ");
                Delay_ms(1000);
        }
}
