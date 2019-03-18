//
// PC11 << UART4_RX << EXE/MDB1_RX
// PA0 >> UART4_TX >> EXE/MDB1_TX
//
// PC12 >> UART5_TX >> EXE/MDB2_TX
// PD2 << UART5_RX << EXE/MDB2_RX
//

void main() {
     UART1_Init(115200);//(stdio/aux3)
     //UART4_Init(9600);//mdb/exe1
     //UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PC10_11); // _GPIO_UART4_TX_PC10, _GPIO_UART4_RX_PC11
     //UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01); // _GPIO_UART4_TX_PA0, _GPIO_UART4_RX_PA1
     UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01_PC11); // this should not work
     UART5_Init(9600);//mdb/exe2
     //UART5_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2); // this should work
     Delay_ms(100);
     UART1_Write_Text("hello123\r\n");
     while(1)
     {
      UART5_Write(0x31);
      Delay_ms(100);
      UART5_Write(0x32);
      Delay_ms(100);
      //UART4_Write(0x42);
      //Delay_ms(100);
     }
}