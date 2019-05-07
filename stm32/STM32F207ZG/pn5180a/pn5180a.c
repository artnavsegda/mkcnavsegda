#include <stdint.h>
#include <stdbool.h>
#include "pcf8574.h"
#include "pn5180a.h"

#define LOW false
#define HIGH true

#define PN5180_BUSY  0b01000000
#define PN5180_RESET 0b10000000

#define PN5180_NSS 0b10000000000

void PrintHandler(char c) {
     UART1_Write(c);
}

bool digitalRead(uint8_t mask)
{
     uint8_t iolines = PCF_RdSingle(0x3F);
     if (iolines & mask)
        return HIGH;
     else
        return LOW;
}

void digitalWrite(uint16_t mask, bool level)
{
     if (level)
        GPIOF_ODR != mask;
     else
        GPIOF_ODR &= ~(mask);
}

bool PN5180_transceiveCommand(uint8_t *sendBuffer, int sendBufferLen, uint8_t *recvBuffer, int recvBufferLen)
{
  uint8_t i;
  // 0.
  while (LOW != digitalRead(PN5180_BUSY)); // wait until busy is low
  // 1.
  digitalWrite(PN5180_NSS, LOW); delay_ms(2);
  // 2.
  for (i=0; i<sendBufferLen; i++) {
    SPI1_Write(sendBuffer[i]);
   }
  // 3.
  while(HIGH != digitalRead(PN5180_BUSY));  // wait until BUSY is high
  // 4.
  digitalWrite(PN5180_NSS, HIGH);
  delay_ms(1);
  // 5.
  while (LOW != digitalRead(PN5180_BUSY)); // wait unitl BUSY is low

  // check, if write-only
  //
  if ((0 == recvBuffer) || (0 == recvBufferLen)) return true;

  // 1.
  digitalWrite(PN5180_NSS, LOW);
  delay_ms(2);
  // 2.
  for (i=0; i<recvBufferLen; i++) {
    recvBuffer[i] = SPI1_Read(0xff);
  }
  // 3.
  while(HIGH != digitalRead(PN5180_BUSY));  // wait until BUSY is high
  // 4.
  digitalWrite(PN5180_NSS, HIGH);
  delay_ms(1);
  // 5.
  while(LOW != digitalRead(PN5180_BUSY));  // wait until BUSY is low

  return true;
}

bool PN5180A_readEEprom(uint8_t addr, uint8_t *buffer, uint8_t len)
{
  uint8_t cmd[3];
  cmd[0] = PN5180_READ_EEPROM;
  cmd[1] = addr;
  cmd[2] = len;
  if ((addr > 254) || ((addr+len) > 254)) {
    return false;
  }
  PN5180_transceiveCommand(cmd, 3, buffer, len);

  return true;
}

void PN5180A_Reset(void)
{
     PCF_WrSingle(0x3F, 0b11111111); //all pins pull high
     PCF_WrSingle(0x3F, 0b01111111); //IO7(NFC_RESET) pull low
     Delay_us(10); // delay 10 us
     PCF_WrSingle(0x3F, 0b11111111); //all pins pull high
}

bool PN5180A_loadRFConfig(uint8_t txConf, uint8_t rxConf) {
  uint8_t cmd[3];
  cmd[0] = PN5180_LOAD_RF_CONFIG;
  cmd[1] = txConf;
  cmd[2] = rxConf;
  PN5180_transceiveCommand(cmd, 3, 0, 0);

  return true;
}

bool PN5180A_readRegister(uint8_t reg, uint32_t *value) {
  uint8_t cmd[2];
  cmd[0] = PN5180_READ_REGISTER;
  cmd[1] = reg;

  PN5180_transceiveCommand(cmd, 2, (uint8_t*)value, 4);

  return true;
}

uint32_t PN5180A_getIRQStatus() {
  uint32_t irqStatus;
  PN5180A_readRegister(IRQ_STATUS, &irqStatus);

  return irqStatus;
}

bool PN5180A_writeRegister(uint8_t reg, uint32_t value) {
  uint8_t *p = (uint8_t*)&value;
  uint8_t buf[6];
  buf[0] = PN5180_WRITE_REGISTER;
  buf[1] = reg;
  buf[2] = p[0];
  buf[3] = p[1];
  buf[4] = p[2];
  buf[5] = p[3];
  PN5180_transceiveCommand(buf, 6, 0, 0);

  return true;
}

bool PN5180A_clearIRQStatus(uint32_t irqMask) {

  return PN5180A_writeRegister(IRQ_CLEAR, irqMask);
}

bool PN5180A_setRF_on() {
  uint8_t cmd[2];
  cmd[0] = PN5180_RF_ON;
  cmd[1] = 0x00;

  PN5180_transceiveCommand(cmd, 2, 0 ,0);

  while (0 == (TX_RFON_IRQ_STAT & PN5180A_getIRQStatus())); // wait for RF field to set up
  PN5180A_clearIRQStatus(TX_RFON_IRQ_STAT);
  return true;
}

bool PN5180A_writeRegisterWithAndMask(uint8_t reg, uint32_t mask) {
  uint8_t *p = (uint8_t*)&mask;

  uint8_t buf[6];
  buf[0] = PN5180_WRITE_REGISTER_AND_MASK;
  buf[1] = reg;
  buf[2] = p[0];
  buf[3] = p[1];
  buf[4] = p[2];
  buf[5] = p[3];

  PN5180_transceiveCommand(buf, 6, 0, 0);

  return true;
}

bool PN5180A_writeRegisterWithOrMask(uint8_t reg, uint32_t mask) {
  uint8_t *p = (uint8_t*)&mask;

  uint8_t buf[6];
  buf[0] = PN5180_WRITE_REGISTER_OR_MASK;
  buf[1] = reg;
  buf[2] = p[0];
  buf[3] = p[1];
  buf[4] = p[2];
  buf[5] = p[3];

  PN5180_transceiveCommand(buf, 6, 0, 0);

  return true;
}

bool PN5180A_setupRF() {
  if (PN5180A_loadRFConfig(0x0d, 0x8d)) {  // ISO15693 parameters
    ;
  }
  else return false;

  if (PN5180A_setRF_on()) {
    ;
  }
  else return false;

  PN5180A_writeRegisterWithAndMask(SYSTEM_CONFIG, 0xfffffff8);  // Idle/StopCom Command
  PN5180A_writeRegisterWithOrMask(SYSTEM_CONFIG, 0x00000003);   // Transceive Command

  return true;
}

void main() {
     uint8_t productVersion[2];
     uint8_t firmwareVersion[2];
     uint8_t eepromVersion[2];

     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9); // i2c start
     SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1, &_GPIO_MODULE_SPI1_PA56_PB5);
     GPIO_Digital_Output(&GPIOF_BASE, _GPIO_PINMASK_10); // Set PORTF pin 10 as digital output, SPI_CS_NFC_NSS
     UART1_Init(9600); //(stdio/aux3)
     UART1_Write_Text("start nfc test\r\n");
     
     PN5180A_readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
     PN5180A_readEEprom(FIRMWARE_VERSION, firmwareVersion, sizeof(firmwareVersion));
     PN5180A_readEEprom(EEPROM_VERSION, eepromVersion, sizeof(eepromVersion));
     PN5180A_setupRF();
     
     while(1)
     {

     }
}