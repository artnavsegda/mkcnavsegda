#include "pcf8574.h"

void PrintHandler(char c) {
     UART1_Write(c);
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
     UART1_Init(9600); //(stdio/aux3)
     UART1_Write_Text("start nfc test\r\n");
}