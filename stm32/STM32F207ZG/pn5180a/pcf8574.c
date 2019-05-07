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
