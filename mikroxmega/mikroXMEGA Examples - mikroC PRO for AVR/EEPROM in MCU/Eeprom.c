/*
 * Project name:
     Eeprom (Demonstration of using EEPROM Library)
 * Copyright:
     (c) Mikroelektronika, 2010.
 * Revision History:
     20100610:
       - initial release;
 * Description:
     This is a demonstration of using library functions for handling of the
     AVR's internal EEPROM module. First, some data is written to EEPROM
     then the data is read from the same locations and displayed on UARTC0
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

char ii;
char dta;

void main(){
   UARTC0_Init(19200);                  // Initialize UARTC0 at 19200 bps

   for(ii = 0; ii < 0xFF; ii++)         // EEPROM write loop
     EEPROM_Write(0x100+ii, ii);        // Write data to address 0x100+ii

   for(ii = 0; ii < 0xFF; ii++) {       // EEPROM read loop
     dta = EEPROM_Read(0x100+ii);       // Read data from address 0x100+ii
     UARTC0_Write(dta);                 //   and display it on UARTC0
   }
}