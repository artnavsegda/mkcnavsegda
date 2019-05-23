#include "pcf8574.h"

void main() {
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9); // i2c start
     PCF_WrSingle(0x3F,0xFF);
     SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1, &_GPIO_MODULE_SPI1_PA56_PB5);
     GPIO_Digital_Output(&GPIOF_BASE, _GPIO_PINMASK_8 | _GPIO_PINMASK_9 | _GPIO_PINMASK_10 |_GPIO_PINMASK_11); // Set PORTF pin 8,9,10,11 as digital output, SPI_CS0, SPI_CS1, SPI_CS2, SPI_CS3
     GPIOF_ODR |= _GPIO_PINMASK_11 | _GPIO_PINMASK_10 | _GPIO_PINMASK_8; // CS2 output LOW
     UART1_Init(9600); //(stdio/aux3)
     UART1_Write_Text("start nfc test\r\n");
}