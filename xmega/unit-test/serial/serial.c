void PrintHandler(char c)
{
        UART_Write(c);
}

void main() {
        UARTC0_Init(9600);
        //UARTC1_Init(9600);
        //UARTD0_Init(9600);
        //UARTD1_Init(9600);
        //UARTE0_Init(9600);
        //UARTF0_Init(9600);
        
         while (1)
        {
                UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
                PrintOut(PrintHandler, "Hello %x\r\n", 0xDEAD);
                delay_ms(1000);
                //UARTC0_Write(0xFF);
                //UART_Set_Active(&UARTC1_Read, &UARTC1_Write, &UARTC1_Data_Ready, &UARTC1_Tx_Idle);
                //UARTC1_Write(0xFF);
                //UART_Set_Active(&UARTD0_Read, &UARTD0_Write, &UARTD0_Data_Ready, &UARTD0_Tx_Idle);
                //UARTD0_Write(0xFF);
                //UART_Set_Active(&UARTD1_Read, &UARTD1_Write, &UARTD1_Data_Ready, &UARTD1_Tx_Idle);
                //UARTD1_Write(0xFF);
                //UART_Set_Active(&UARTE0_Read, &UARTE0_Write, &UARTE0_Data_Ready, &UARTE0_Tx_Idle);
                //UARTE0_Write(0xFF);
                //UART_Set_Active(&UARTF0_Read, &UARTF0_Write, &UARTF0_Data_Ready, &UARTF0_Tx_Idle);
                //UARTF0_Write(0xFF);
        }
}