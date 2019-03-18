//
// PC11 << UART4_RX << EXE/MDB1_RX
// PA0 >> UART4_TX >> EXE/MDB1_TX
//
// PC12 >> UART5_TX >> EXE/MDB2_TX
// PD2 << UART5_RX << EXE/MDB2_RX
//

unsigned long long num = 0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

void Uart5_interrupt() iv IVT_INT_UART5 ics ICS_AUTO {
     char string[10];
     if (UART5_Data_Ready())
     {
      sprintf(string,"RX5 %X %lu\r\n",UART5_Read(),num);
      UART1_Write_Text(string);
     }
}

void main() {
     char string[10];

     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 15999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer


     UART1_Init(115200);//(stdio/aux3)
     //UART4_Init(9600);//mdb/exe1
     //UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PC10_11); // _GPIO_UART4_TX_PC10, _GPIO_UART4_RX_PC11
     //UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01); // _GPIO_UART4_TX_PA0, _GPIO_UART4_RX_PA1
     UART4_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01_PC11); // this should not work
     UART5_Init(9600);//mdb/exe2
     
     UART5_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_UART5); // enable interrupt vector
     
     //UART5_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2); // this should work
     Delay_ms(100);
     UART1_Write_Text("hello123\r\n");
     while(1)
     {
      sprintf(string,"TX5 31 %lu\r\n",num);
      UART5_Write(0x31);
      UART1_Write_Text(string);
      
      Delay_ms(100);
      
      sprintf(string,"TX5 32 %lu\r\n",num);
      UART5_Write(0x32);
      UART1_Write_Text(string);
      
      Delay_ms(100);
     }
}