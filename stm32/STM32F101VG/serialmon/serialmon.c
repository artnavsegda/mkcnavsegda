long long num = 0;

void PrintHandler(char c) {
  UART4_Write(c);
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

// Interrupt routine
void Uart1_interrupt() iv IVT_INT_USART1 ics ICS_AUTO {
     if (UART1_Data_Ready())
      PrintOut(PrintHandler,"RX1 %X %u\r\n",UART1_Read(),num);
}

// Interrupt routine
void Uart3_interrupt() iv IVT_INT_USART3 ics ICS_AUTO {
     if (UART3_Data_Ready())
      PrintOut(PrintHandler,"RX3 %X %lu\r\n",UART3_Read(),num);
}

void main() {
     char exetx, exerx;
     char string[10];
     int i = 0;
     
     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 7999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer
     
     UART1_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);    //(RX+/-)
     USART1_CR1bits.M = 1; //9 bit data transfer
     USART1_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART1); // enable interrupt vector

     UART3_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART3_PB10_11);    //(TX+/-)
     USART3_CR1bits.M = 1; //9 bit data transfer
     USART3_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART3); // enable interrupt vector
     
     UART4_Init(115200);//stdio
     Delay_ms(100);
     UART4_Write_Text("starting\r\n");
     Delay_ms(100);
     while(1);
}