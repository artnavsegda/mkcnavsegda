void main() {
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_5); // Set PORTE pin 5 as digital output, GSM_PW_ON

  //GPIOC_ODR = 0;
  
  //ODR5_GPIOE_ODR_bit = 1; // enable GSM forever
  
  ODR5_GPIOE_ODR_bit = 0; // disable GSM forever

  while(1) {
    Delay_ms(5000);
/*//GPIOC_ODR = ~GPIOC_ODR; // Toggle PORTC
    ODR5_GPIOE_ODR_bit = 0;
    Delay_ms(5000);
    ODR5_GPIOE_ODR_bit = 1;*/
  }
}