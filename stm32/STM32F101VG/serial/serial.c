unsigned long long num = 0;

void PrintHandler(char c) {
  UART4_Write(c);
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     GPIOD_ODR = ~GPIOD_ODR;      // Toggle PORTE led's
     num++;
}

void main() {

     GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_ALL);  // Enable digital output on PORTD
     GPIOD_ODR = 0;

     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 7999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer

     UART4_Init(9600);
     Delay_ms(100);
     
     PrintOut(PrintHandler,"USART 4 BRR register is 0x%X\r\n",UART4_BRR);
     PrintOut(PrintHandler,"USART 4 CR1 register is 0x%X\r\n",UART4_CR1);
     
     UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_EVENPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);    //(RX+/-)
     USART1_CR1.PCE = 1; // parity control enable
     USART1_CR1.PS = 0; // parity even
     Delay_ms(100);
 
     PrintOut(PrintHandler,"USART 1 BRR register is 0x%X\r\n",USART1_BRR);
     PrintOut(PrintHandler,"USART 1 CR1 register is 0x%X\r\n",USART1_CR1);

     while(1)
     {
      //PrintOut(PrintHandler,"test %lu\r\n",num);
      Delay_ms(100);
     }
}