/*
 * Project name:
      Flash MCU test (Demonstration of using Flash Library)
 * Copyright:
     (c) Mikroelektronika, 2010.
 * Revision History:
     20100610:
       - initial release;
 * Description:
     This program demonstrates how to use Flash MCU library.
 * Test configuration:
     MCU:             ATxmega128A1
                      http://www.atmel.com/dyn/resources/prod_documents/doc8067.pdf
     Dev.Board:       -
     Oscillator:      Internal Clock 2.0000 MHz
     Ext. Modules:    -
     SW:              mikroC PRO for AVR
                      http://www.mikroe.com/eng/products/view/228/mikroc-pro-for-avr/
 * NOTES:
     -
 */

const long F_ADDRESS = 0x200;

const unsigned int data_[32] = {                               // constant table
  0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,     //   in Flash at address F_ADDRESS
  0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
  0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700,
  0x0800,0x0900,0x0A00,0x0B00,0x0C00,0x0D00,0x0E00,0x0F00,
} absolute F_ADDRESS;

char i;
unsigned int word_;
char tmp;
unsigned int dat_buff[32];

void main() {

  UARTC0_Init(19200);                          // Initialize UART at 19200 bps
  
  word_ = data_[0];                            //  link const table

  for (i = 0; i<64 ; i+=2) {                   // reading 32 words in loop
    word_ = FLASH_Read_Word(F_ADDRESS + i);    // demonstration of reading single word
    UARTC0_Write(word_);                       // Write on UARTC0
    UARTC0_Write(word_ >> 8);
    Delay_ms(100);
  }

  Delay_ms(2000);

  i = 0;                                       // initialize loop variable
  while ( i < 64 ) {                           // reading 64 bytes in loop
    tmp = FLASH_Read_Byte(F_ADDRESS + i++);    // demonstration of reading single byte
    UARTC0_Write(tmp);                         // Write on UARTC0
    tmp = FLASH_Read_Byte(F_ADDRESS + i++);    // demonstration of reading single byte
    UARTC0_Write(tmp);                         // Write on UARTC0
    Delay_ms(100);
  }

  Delay_ms(2000);

  FLASH_Read_Bytes(F_ADDRESS, dat_buff, 64);   // demonstration of reading 64 bytes block
  for (i = 0; i<32 ; i++) {                    // use loop to display read block
    tmp = dat_buff[i];
    UARTC0_Write(tmp);                         // Write on UARTC0
    tmp = dat_buff[i] >> 8;
    UARTC0_Write(tmp);                         // Write on UARTC0
    Delay_ms(100);
  }

  Delay_ms(2000);

  FLASH_Read_Words(F_ADDRESS, dat_buff, 32);   //  demonstration of reading 32 words block
  for (i = 0; i<32 ; i++) {                    // use loop to display read block
    tmp = dat_buff[i];
    UARTC0_Write(tmp);                         // Write on UARTC0
    tmp = dat_buff[i] >> 8;
    UARTC0_Write(tmp);                         // Write on UARTC0
    Delay_ms(100);
  }

}