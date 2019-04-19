char data_[256];


//--------------- Writes data to 24C02 EEPROM - signle location
void EEPROM_24C02_WrSingle(unsigned short wAddr, unsigned short wData) {
  data_[0] = wAddr;
  data_[1] = wData;
  I2C1_Start();
  // issue I2C start signal
  I2C1_Write(0x53,data_,2,END_MODE_STOP);
}//~

//--------------- Reads data from 24C02 EEPROM - single location (random)
unsigned short EEPROM_24C02_RdSingle(unsigned short rAddr) {
  data_[0] = rAddr;
  I2C1_Start();              // issue I2C start signal
  I2C1_Write(0x53,data_,1,END_MODE_RESTART);
  I2C1_Read(0x53,data_,1,END_MODE_STOP);

  return data_[0];
}

void main(){
  unsigned short i;

  GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_ALL);

  I2C1_Init();        // performs I2C initialization

  GPIOD_ODR = 0xFFFF;
  Delay_ms(1000);

  for(i = 0; i<0x80; i++) {
    EEPROM_24C02_WrSingle(i,i);
    GPIOD_ODR++;
    delay_ms(5);
  }

  Delay_ms(500);
  GPIOD_ODR = 0xFFFF;
  Delay_ms(1000);

  for(i = 0x00; i < 0x80; i++){
    GPIOD_ODR = EEPROM_24C02_RdSingle(i);
    delay_ms(100);
  }

}