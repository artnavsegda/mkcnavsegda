//
// I2C_SCL << I2C3_SCL << PA8
// I2C_SDA << I2C3_SDA << PC9
//

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

unsigned char PCF_RdSingle(unsigned char wAddr)
{
      unsigned char buf[1];
      I2C3_Start();
      I2C3_Read(wAddr,buf,1,END_MODE_STOP);
      return buf[0];
}

void main() {
     //I2C3_Init();
     UART1_Init(9600); //(stdio/aux3)
     UART1_Write_Text("start PCF test\r\n");
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9);

     while(1)
     {
      Delay_ms(1000);
      PrintOut(PrintHandler, "DD3 readout %X\r\n",PCF_RdSingle(0x3F));
     }

/*while(1)
     {
      PCF_WrSingle(0x3C,0x00);
      PCF_WrSingle(0x3D,0x00);
      PCF_WrSingle(0x3E,0x00);
      Delay_ms(1000);
      PCF_WrSingle(0x3C,0xFF);
      PCF_WrSingle(0x3D,0xFF);
      PCF_WrSingle(0x3E,0xFF);
      Delay_ms(1000);
     }*/
}