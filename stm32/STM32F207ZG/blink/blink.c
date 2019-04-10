void main() {
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_ALL); // Set PORTC as digital output

  //GPIOC_ODR = 0;

  while(1) {
    Delay_ms(5000);
    //GPIOC_ODR = ~GPIOC_ODR; // Toggle PORTC
    ODR15_GPIOC_ODR_bit = 0;
    Delay_ms(5000);
    ODR15_GPIOC_ODR_bit = 1;
  }
}