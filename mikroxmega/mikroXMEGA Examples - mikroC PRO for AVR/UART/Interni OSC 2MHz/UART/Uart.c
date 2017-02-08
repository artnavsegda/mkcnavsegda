/*
 * Project name:
     UART (Simple usage of UART module library functions)
 * Copyright:
     (c) Mikroelektronika, 2010.
 * Revision History:
     20080930:
       - initial release;
 * Description:
     This code demonstrates how to use UART library routines. Upon receiving
     data via RS232, MCU immediately sends it back to the sender.
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

char uart_rd;
void main() {

  UARTC0_Init(19200);              // Initialize UART module at 19200 bps
  Delay_ms(1000);                  // Wait for UART module to stabilize

  UARTC0_Write_Text("Start");

  while (1) {                      // Endless loop
   if (UARTC0_Data_Ready()) {      // If data is received,
     uart_rd = UARTC0_Read();      // read the received data,
     UARTC0_Write(uart_rd);        // and send data via UART
    }
  }

}