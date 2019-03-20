//
// PC11 << UART4_RX << EXE/MDB1_RX
// PA0 >> UART4_TX >> EXE/MDB1_TX
//
// PC12 >> UART5_TX >> EXE/MDB2_TX
// PD2 << UART5_RX << EXE/MDB2_RX
//

unsigned long long num = 0;

void PrintHandler(char c) {
     UART1_Write(c);
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

void Uart4_interrupt() iv IVT_INT_UART4 ics ICS_AUTO {
     unsigned rxdata;
     if (UART4_Data_Ready())
     {
      rxdata = UART4_Read();
      UART5_Write(rxdata);
      PrintOut(PrintHandler,"RX4 %X %lu\r\n",rxdata,num);
     }
}

void Uart5_interrupt() iv IVT_INT_UART5 ics ICS_AUTO {
     unsigned rxdata;
     if (UART5_Data_Ready())
     {
      rxdata = UART5_Read();
      UART4_Write(rxdata);
      PrintOut(PrintHandler,"RX5 %X %lu\r\n",rxdata,num);
     }
}

PCF_WrSingle(unsigned char wAddr, unsigned char wData)
{
     unsigned char buf[1];
     buf[0] = wData;
     I2C3_Start();
     I2C3_Write(wAddr,buf,1,END_MODE_STOP);
}

void main() {

     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 15999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer

     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9);
     
     PCF_WrSingle(0x20, 0b00010000); // first(exe1_m/s) port master second(mdb/exe2_m/s) port slave
     //PCF_WrSingle(0x20, 0b01000000); //first(exe1_m/s) port slave second(mdb/exe2_m/s) port master
     //PCF_WrSingle(0x20, 0b00000000); //both ports slaves
     //PCF_WrSingle(0x20, 0b01010000); //both ports masters

     UART1_Init(115200);//(stdio/aux3)

     UART4_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01_PC11); // this should not work //mdb/exe1
     UART4_CR1bits.M = 1; //9 bit data transfer
     UART4_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_UART4); // enable interrupt vector
     
     UART5_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2); //mdb/exe2
     UART5_CR1bits.M = 1; //9 bit data transfer
     UART5_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_UART5); // enable interrupt vector
     
     Delay_ms(100);
     UART1_Write_Text("hello123\r\n");
     while(1)
     {
/*PrintOut(PrintHandler,"TX4 31 %lu\r\n",num);
      UART4_Write(0x31);

      Delay_ms(100);

      PrintOut(PrintHandler,"TX4 32 %lu\r\n",num);
      UART4_Write(0x32);

      Delay_ms(100);

      PrintOut(PrintHandler,"TX5 31 %lu\r\n",num);
      UART5_Write(0x31);

      Delay_ms(100);

      PrintOut(PrintHandler,"TX5 32 %lu\r\n",num);
      UART5_Write(0x32);*/

      Delay_ms(100);
     }
}