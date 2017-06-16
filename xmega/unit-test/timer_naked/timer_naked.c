void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
  PORTD_OUTTGL.B4 = 1;
}

void main()
{
  PORTD_DIRSET.B4 = 1;
  TCC0_PERL = 0x10;                 // Set period 10000
  TCC0_PERH = 0x27;
  TCC0_CTRLA = 0x05;                // Prescaler DIV64
  TCC0_INTCTRLA = 2;                // Enable overflow interrupt
  PMIC_CTRL = 2;                    // Enable medium level interrupts
  CPU_SREG.B7 = 1;                  // Enable global interrupts

  while (1);
}
