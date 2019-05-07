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

void main() {
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9); // i2c start
     SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1, &_GPIO_MODULE_SPI1_PA56_PB5);
     GPIO_Digital_Output(&GPIOF_BASE, _GPIO_PINMASK_10); // Set PORTF pin 10 as digital output, SPI_CS_NFC_NSS
     UART1_Init(9600); //(stdio/aux3)
     UART1_Write_Text("start nfc test\r\n");
}