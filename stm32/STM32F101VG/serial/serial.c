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
     char string[10];
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

     while(1)
     {
      PrintOut(PrintHandler,"test %lu\r\n",num);
      Delay_ms(100);
     }
}