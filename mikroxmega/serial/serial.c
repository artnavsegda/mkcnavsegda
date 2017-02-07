void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        char *ip, *mac;
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);

         while (1)
        {
                //PrintOut(PrintHandler, "Hello %x\r\n", 0xDEAD);
                //delay_ms(1000);
        }
}
