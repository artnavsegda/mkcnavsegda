void main() {
//  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_ALL); // Set PORTA as digital output
//  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_ALL); // Set PORTB as digital output
//  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_ALL); // Set PORTC as digital output
  GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_ALL); // Set PORTD as digital output
//  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_ALL); // Set PORTE as digital output

//  GPIOA_ODR = 0;
//  GPIOB_ODR = 0;
//  GPIOC_ODR = 0;
  GPIOD_ODR = 0;
//  GPIOE_ODR = 0;

  while(1) {
//    GPIOA_ODR = ~GPIOA_ODR; // Toggle PORTA
//    GPIOB_ODR = ~GPIOB_ODR; // Toggle PORTB
//    GPIOC_ODR = ~GPIOC_ODR; // Toggle PORTC
    GPIOD_ODR = ~GPIOD_ODR; // Toggle PORTD
//    GPIOE_ODR = ~GPIOE_ODR; // Toggle PORTE
    Delay_ms(100);
  }
}