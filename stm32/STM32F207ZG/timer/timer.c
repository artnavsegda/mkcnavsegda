unsigned long long num = 0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
  TIM2_SR.UIF = 0;
  num++;
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
     
     UART1_Init(9600);
     
     while(1)
     {
             sprintf(string,"test %lu\r\n",num);
             UART1_Write_Text(string);
             Delay_ms(100);
     }
}
