void Timer2_interrupt() iv IVT_INT_TIM2 {
  TIM2_SR.UIF = 0;
  GPIOE_ODR = ~GPIOE_ODR;      // Toggle PORTE led's
}

void main() {
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_ALL);  // Enable digital output on PORTE
  GPIOE_ODR = 0;

  RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
  TIM2_CR1.CEN = 0;             // Disable timer
  TIM2_PSC = 1098;              // Set timer prescaler.
  TIM2_ARR = 65514;
  NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
  TIM2_DIER.UIE = 1;            // Update interrupt enable
  TIM2_CR1.CEN = 1;             // Enable timer

  while(1)  ;
}
