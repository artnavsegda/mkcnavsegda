void main() {
     UART1_Init(115200);//(stdio/aux3)
     //UART4_Init(9600);//mdb/exe1
     UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PC10_11);
     UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01);
     UART5_Init(9600);//mdb/exe2
     Delay_ms(100);
     UART1_Write_Text("hello123\r\n");
     while(1)
     {

      Delay_ms(100);
     }
}