//
// I2C_SCL << I2C3_SCL << PA8
// I2C_SDA << I2C3_SDA << PC9
//

unsigned long long num = 0;

PCF_WrSingle(unsigned char wAddr, unsigned char wData)
{
     unsigned char buf[1];
     buf[0] = wData;
     I2C3_Start();
     I2C3_Write(wAddr,buf,1,END_MODE_STOP);
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

void main() {

     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 15999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer


     //I2C3_Init();
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9);
     
     Delay_ms(100);

     while(1)
     {
      PCF_WrSingle(0x3E,0xFF);
      PCF_WrSingle(0x3C,0xFC);
      Delay_ms(1000);
      PCF_WrSingle(0x3C,0xFF);
      PCF_WrSingle(0x3E,0xF3);
      Delay_ms(1000);
     }
}