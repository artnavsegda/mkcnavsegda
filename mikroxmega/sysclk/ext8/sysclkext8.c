void main() {
// ----- External Clock selection procedure!!!
   OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
   OSC_CTRL |= 0x08;      // enable external oscillator

   while(XOSCRDY_bit == 0)
    ;

   CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
                    // I/O register or execution of the protected instruction for a maximum of 4 CPU instruction cycles.
   CLK_CTRL = 3;    // External oscillator
// ---------------------------------
}