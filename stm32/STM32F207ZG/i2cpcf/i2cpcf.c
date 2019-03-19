//
// I2C_SCL << I2C3_SCL << PA8
// I2C_SDA << I2C3_SDA << PC9
//

PCF_WrSingle(unsigned short wAddr, unsigned short wData)
{
     I2C3_Start();
     I2C3_Write(wAddr,wData,1,END_MODE_STOP);
}

void main() {
     I2C3_Init();
     
     while(1)
     {
      PCF_WrSingle(0x00,0x00);
      Delay_ms(100);
      PCF_WrSingle(0x00,0xFF);
      Delay_ms(100);
     }
}
