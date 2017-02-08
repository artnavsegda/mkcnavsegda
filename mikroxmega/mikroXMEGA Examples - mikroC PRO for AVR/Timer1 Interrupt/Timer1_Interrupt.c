/*
 * Project name:
     Timer1_Interrupt (Using Timer1 to obtain interrupts)
 * Copyright:
     (c) Mikroelektronika, 2010.
 * Revision History:
     20100610:
       - initial release;
 * Description:
     This code demonstrates how to use Timer1 and it's interrupt.
     Program toggles PORTD.
 * Test configuration:
     MCU:             ATxmega128A1
                      http://www.atmel.com/dyn/resources/prod_documents/doc8067.pdf
     Dev.Board:       -
     Oscillator:      Internal Clock 2.0000 MHz
     Ext. Modules:    -
     SW:              mikroC PRO for AVR
                      http://www.mikroe.com/eng/products/view/228/mikroc-pro-for-avr/
 * NOTES:
     - Toggles PORTD.
*/

void Timer1Overflow_ISR() org IVT_ADDR_TCC1_OVF {
  PORTD_OUTTGL = 0xFF;             // Toggle PORTD
}

void main() {
  PORTD_DIRSET = 0xFF;              // Set PORTD as output
  PORTD_OUTCLR = 0xFF;              // Clear PORTD

  delay_ms(1000);

  TCC1_PERL = 0x10;                 // Set period 10000
  TCC1_PERH = 0x27;
  TCC1_CTRLA = 0x05;                // Prescaler DIV64
  TCC1_INTCTRLA = 2;                // Enable overflow interrupt
  PMIC_CTRL = 2;                    // Enable medium level interrupts
  CPU_SREG.B7 = 1;                  // Enable global interrupts

  while (1)                         // Endless loop, port is changed inside Interrupt Service Routine (ISR)
    ;
}