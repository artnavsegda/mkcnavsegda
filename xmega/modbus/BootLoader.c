 /*
 * Project name:
     Bootloader Example
 * Copyright:
     (c) Mikroelektronika, 2010.
 * Revision History:
     20101119:
       - initial release;
 * Description:
     This is a serial port Bootloader implementation.
     Once started in MCU, the bootloader waits for a while (approx. 5 seconds)
     to establish UART communication with the MikroBootloader application on the
     PC. If it fails to do so, the bootloader starts the programme already
     loaded in the MCU. If the communication is established, the bootloader
     receives the hex file from the PC and places it where requred. When hex download
     has finished, the user is notified by the MikroBootloader to reset the MCU,
     and MCU enters the described bootload sequence again.
 * Test configuration:
     MCU:             ATxmega128A1
                      http://www.atmel.com/dyn/resources/prod_documents/doc8077.pdf
     Dev.Board:       XMEGA Ready
                      http://www.mikroe.com/ready/xmega/
     Oscillator:      Internal Clock 02.0000 MHz
     Ext. Modules:    -
     SW:              mikroC PRO for AVR
                      http://www.mikroe.com/mikroc/avr/
 * NOTES:
     -
*/

#pragma orgall 0x010000   // Place all above specified address
#include "built_in.h"

const int __FLASH_PAGE_SIZE = 256;

const unsigned int BOOTLOADER_START_ADDRESS = 0x010000;  // Address where bootloader is placed
const int FLASH_PAGE_SIZE_BYTES = __FLASH_PAGE_SIZE*2;  // size in bytes
static unsigned int block[__FLASH_PAGE_SIZE];

void __BootDelay(){
  delay_ms(5);
}

// Starts the main of loaded program
void Start_Program(){
  asm JMP 0;
}

// This function will send a char over the USART until another char is recieved
// or until a timeout is reached. If the correct char is recieved the function will
// return 1 else it will return 0
unsigned short UART_Write_Loop(char send, char receive) {
  unsigned short rslt = 0;

  while(1){
    __BootDelay();
    UARTC0_Write(send);
    __BootDelay();

    rslt++;
    if (rslt == 255u)
      return 0;
    if (UARTC0_Read() == receive)
      return 1;
  }
}

//-------------- Starts with bootload
void Start_Bootload() {
  unsigned short xx, yy;
  unsigned int i = 0;
  unsigned long j=0;

  while (1) {
    if (i == __FLASH_PAGE_SIZE) {            // If FLASH_PAGE_SIZE_BYTES bytes recieved then write to flash
      i = 0;
      if (j <( (unsigned long)BOOTLOADER_START_ADDRESS*2)){
        FLASH_Erase_Write_Application_Page(j, block);
      }
      j += FLASH_PAGE_SIZE_BYTES;
    }

    //--- Ask for yy
    UARTC0_Write('y');
    while (!UARTC0_Data_Ready());
    //--- Read yy
    yy = UARTC0_Read();
    //--- Ask for xx
    UARTC0_Write('x');
    while (!UARTC0_Data_Ready()) ;
    //--- Read xx
    xx = UARTC0_Read();
    //--- Save xxyy in block[i]
    Lo(block[i]) = yy;
    Hi(block[i]) = xx;
    i++;
  }
}

void Sysclk_Init(void);

void main() org BOOTLOADER_START_ADDRESS
{
     Sysclk_Init();
     UARTC0_Init(115200);

  if (UART_Write_Loop('g','r')) {   // Send 'g' for ~5 sec, if 'r'
    Start_Bootload();               //   received start bootload
  }
  else {
    Start_Program();                //   else start program
  }
}