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
       PrintOut(PrintHandler,"RX5 %X\r\n",rxdata);
       switch (rxdata)
       {
        case 0x131:
             UART5_Write(0x140);
        break;
        case 0x138:
        case 0x120:
        case 0x24:
             UART5_Write(0);
        default:
        break;
       }
      }
     }
}

