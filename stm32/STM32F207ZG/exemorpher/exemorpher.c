//
// PC11 << UART4_RX << EXE/MDB1_RX
// PA0 >> UART4_TX >> EXE/MDB1_TX
//
// PC12 >> UART5_TX >> EXE/MDB2_TX
// PD2 << UART5_RX << EXE/MDB2_RX
//

struct exedata {
       unsigned buv0;
       unsigned buv1;
       unsigned buv2;
       unsigned buv3;
       unsigned sf0;
       unsigned sf1;
       unsigned dpi;
       unsigned ec;
       unsigned datasync;
};

unsigned char button = 1;

unsigned long long num = 0;

PCF_WrSingle(unsigned char wAddr, unsigned char wData)
{
     unsigned char buf[1];
     buf[0] = wData;
     I2C3_Start();
     I2C3_Write(wAddr,buf,1,END_MODE_STOP);
}

unsigned char PCF_RdSingle(unsigned char wAddr)
{
      unsigned char buf[1];
      I2C3_Start();
      I2C3_Read(wAddr,buf,1,END_MODE_STOP);
      return buf[0];
}

void PrintHandler(char c) {
     UART1_Write(c);
}

void checkbutton(void)
{
      unsigned char buttonstate = PCF_RdSingle(0x03F);
      if (!(buttonstate&0x1))
      {
          button = 1;
          PCF_WrSingle(0x3C,0xFF);
          PCF_WrSingle(0x3E,0xF3);
      }
      else if (!(buttonstate&0x2))
      {
          button = 2;
          PCF_WrSingle(0x3E,0xFF);
          PCF_WrSingle(0x3C,0xFC);
      }
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
     checkbutton();
}

void parsexdata(struct exedata * indata)
{
     unsigned buv = 0;
     
     buv = (indata->buv3&0xF)<<12|(indata->buv2&0xF)<<8|(indata->buv1&0xF)<<4|(indata->buv0&0xF);
     PrintOut(PrintHandler,"%d BUV\r\n",buv);
     //PrintOut(PrintHandler,"%X%X%X%X BUV\r\n",indata->buv3&0xF,indata->buv2&0xF,indata->buv1&0xF,indata->buv0&0xF);
     PrintOut(PrintHandler,"%X SF 0\r\n",indata->sf0 & 0xF);
     PrintOut(PrintHandler,"%X SF 1\r\n",indata->sf1 & 0xF);
     PrintOut(PrintHandler,"%X DPI\r\n",indata->dpi & 0xF);
     PrintOut(PrintHandler,"%X EC\r\n",indata->ec & 0xF);
     PrintOut(PrintHandler,"%X DATA SYNC\r\n",indata->datasync & 0xF);
}

void morphexdata(struct exedata * indata)
{
     unsigned buv = 0;
     buv = (indata->buv3&0xF)<<12|(indata->buv2&0xF)<<8|(indata->buv1&0xF)<<4|(indata->buv0&0xF);
     
     if (button == 2)
        buv = buv*2;
     
     indata->buv0 = 0x20 | (buv & 0xF);
     indata->buv1 = 0x20 | ((buv&0xF0)>>4);
     indata->buv2 = 0x20 | ((buv&0xF00)>>8);
     indata->buv3 = 0x20 | ((buv&0xF000)>>12);
     
}

void sendexdata(struct exedata * indata)
{
     unsigned rxdata_slave;
     UART4_Write(0x138); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->buv0); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->buv1); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->buv2); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->buv3); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->sf0); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->sf1); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->dpi); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->ec); rxdata_slave = UART4_Read();//OK
     UART4_Write(indata->datasync); rxdata_slave = UART4_Read();//OK
}

void main() {
     unsigned rxdata;
     unsigned rxdata_slave;
     struct exedata idata;

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
     
     PCF_WrSingle(0x3E,0xF3);

     UART1_Init(115200);//(stdio/aux3)

     UART4_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART4_PA01_PC11); // this should not work //mdb/exe1
     UART4_CR1bits.M = 1; //9 bit data transfer

     UART5_Init_Advanced(9600, _UART_9_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2); //mdb/exe2
     UART5_CR1bits.M = 1; //9 bit data transfer

     Delay_ms(100);
     UART1_Write_Text("hello123\r\n");
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
                UART5_Write(0);
             else
                UART5_Write(0x140);
             UART4_Write(0x131); rxdata_slave = UART4_Read();
        break;
        case 0x132: // 1 001 1 0010 (VMC CREDIT)
             UART5_Write(0x1FE); // no sell
             //UART5_Write(0x101); //sell
             UART4_Write(0x132); rxdata_slave = UART4_Read();
        break;
        case 0x33:  // 0 001 1 0011 (VMC VEND)
             UART5_Write(0);
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
             idata.buv0 = UART5_Read(); UART5_Write(0); //OK
             idata.buv1 = UART5_Read(); UART5_Write(0); //OK
             idata.buv2 = UART5_Read(); UART5_Write(0); //OK
             idata.buv3 = UART5_Read(); UART5_Write(0); //OK
             idata.sf0 = UART5_Read(); UART5_Write(0); //OK
             idata.sf1 = UART5_Read(); UART5_Write(0); //OK
             idata.dpi = UART5_Read(); UART5_Write(0); //OK
             idata.ec = UART5_Read(); UART5_Write(0); //OK
             idata.datasync = UART5_Read(); UART5_Write(0); //OK
             morphexdata(&idata);
             parsexdata(&idata);
             sendexdata(&idata);
        break;

        case 0x151:
        break;

        default:
             PrintOut(PrintHandler,"RX5 %X\r\n",rxdata);
        break;
       }
      }
}
}