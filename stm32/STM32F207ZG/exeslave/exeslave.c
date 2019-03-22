unsigned long long num = 0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

void PrintHandler(char c) {
     UART1_Write(c);
}

PCF_WrSingle(unsigned char wAddr, unsigned char wData)
{
     unsigned char buf[1];
     buf[0] = wData;
     I2C3_Start();
     I2C3_Write(wAddr,buf,1,END_MODE_STOP);
}

void main() {
     unsigned rxdata;
     
     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 15999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1; // Enable timer
     
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9);
     UART1_Init(115200);//(stdio/aux3)
     UART1_Write_Text("hello123\r\n");
     UART5_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2);//mdb/exe2
     UART5_CR1bits.M = 1; //9 bit data transfer
     PCF_WrSingle(0x20, 0b00000000); //both ports slaves
     while(1)
     {
      if (UART5_Data_Ready())
      {
       rxdata = UART5_Read();
       //PrintOut(PrintHandler,"RX5 %X\r\n",rxdata);
       switch (rxdata)
       {
        case 0x131: // 1 001 1 0001 (VMC STATUS)
             if (num > 5000)
                UART5_Write(0); // ACK
             else
                UART5_Write(0x140); // VMC FVR OK
             // UART5_Write(0x128); // Free vend request
        break;
        case 0x132: // 1 001 1 0010 (VMC CREDIT)
             UART5_Write(0x1FE);// no sell    | no vend request
             //UART5_Write(0x101); //sell     | vend request
             //UART5_Write(0x0); //vend price = 0
        break;
        case 0x33: // 1 001 1 0010 (VMC VEND)
             UART5_Write(0);// vend succeded
        break;
        /*case 0x138: // 1 001 1 1000 (VMC ACCEPT DATA)
        case 0x120: // 1 001 0 0000 (BUV 0x0)
        case 0x24:
        case 0x21: // 0 001 0 0001 (EC 1) -> OK
        case 0x39: // 0 001 1 1001 (DATA SYNC)
             UART5_Write(0);
             PrintOut(PrintHandler,"RX5 %X\r\n",rxdata);
        break;*/
        
        case 0x138: // 1 001 1 1000 (VMC ACCEPT DATA)
             UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X BUV 0\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X BUV 1\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X BUV 2\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X BUV 3\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X SF 0\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X SF 1\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X DPI\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X EC\r\n",rxdata);UART5_Write(0); //OK
             rxdata = UART5_Read(); PrintOut(PrintHandler,"RX5 %X DATA SYNC\r\n",rxdata);UART5_Write(0); //OK
        break;
        
        default:
             PrintOut(PrintHandler,"RX5 %X\r\n",rxdata);
        break;
       }
      }
     }
}