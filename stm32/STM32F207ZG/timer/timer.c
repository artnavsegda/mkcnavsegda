void Timer2_interrupt() iv IVT_INT_TIM2 {
  TIM2_SR.UIF = 0;
}

void main() {

  RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
  TIM2_CR1.CEN = 0;             // Disable timer
  TIM2_PSC = 0;              // Set timer prescaler.
  TIM2_ARR = 15999;
  NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
  TIM2_DIER.UIE = 1;            // Update interrupt enable
  TIM2_CR1.CEN = 1;             // Enable timer

  while(1)  ;
}
