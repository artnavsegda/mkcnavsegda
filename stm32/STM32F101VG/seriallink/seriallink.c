long long num = 0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

// Interrupt routine
void Uart1_interrupt() iv IVT_INT_USART1 ics ICS_AUTO {
     char string[10];
     if (UART1_Data_Ready())
     {
      sprintf(string,"RX1 %X %lu\r\n",UART1_Read(),num);
      UART4_Write_Text(string);
     }
}

// Interrupt routine
void Uart3_interrupt() iv IVT_INT_USART3 ics ICS_AUTO {
     char string[10];
     if (UART3_Data_Ready())
     {
      sprintf(string,"RX1 %X %lu\r\n",UART3_Read(),num);
      UART4_Write_Text(string);
     }
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
     
     
     
     UART1_Init(9600);//(RX+/-)
     
     USART1_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART1); // enable interrupt vector
     
     UART3_Init(9600);//(TX+/-)
     
     USART3_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART3); // enable interrupt vector
     
     UART4_Init(115200);//stdio
     Delay_ms(100);
     UART4_Write_Text("starting\r\n");
     Delay_ms(100);
     while(1);
}