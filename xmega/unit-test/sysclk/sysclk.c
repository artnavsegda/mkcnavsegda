void main() {

	// 2 mhz calibrated by internal 32khz rc generator
//        OSC_CTRL.RC32KEN = 1; // enable 32 khz rc gen
//        while(RC32MRDY_bit == 0);// wait 32 khz rc to go in
//	DFLLRC2M_CTRL.ENABLE = 1;

        // 2 mhz calibrated by rtc crystal
 //       OSC_DFLLCTRL.RC2MCREF = 1; // set 2 mhz calibration source to external osc
//       DFLLRC2M_CTRL.ENABLE = 1; // enable 32 mhz rc gen DFFL auto calibraion

        // 32 mhz calibrated by rtc crystal
/*     OSC_CTRL.RC32MEN = 1; // enable 32 mhz rc gen
     while(RC32MRDY_bit == 0); // wait 32 mhz rc to go in
     OSC_DFLLCTRL.RC32MCREF0 = 1; // set 32 mhz calibration source to extarnal osc
     DFLLRC32M_CTRL.ENABLE = 1; // enable 32 mhz rc gen DFFL auto calibraion
     CPU_CCP = 0xD8; // unlock security bits
     CLK_CTRL.SCLKSEL0 = 1; // select 32 mhz rc generator as system clock
     OSC_CTRL.RC2MEN = 0; // disable 2 mhz rc gen*/

/*        // 32 mhz clock calibrated by internal rc generator
     OSC_CTRL.RC32MEN = 1; // enable 32 mhz rc gen
     OSC_CTRL.RC32KEN = 1; // enable 32 khz rc gen
     while(RC32KRDY_bit == 0);// wait 32 mhz rc to go in
     while(RC32MRDY_bit == 0);// wait 32 khz rc to go in
     DFLLRC32M_CTRL.ENABLE = 1;// enable 32 mhz rc gen DFFL auto calibraion
     CPU_CCP = 0xD8;// unlock security bits
     CLK_CTRL.SCLKSEL0 = 1;// select 32 mhz rc generator as system clock
     OSC_CTRL.RC2MEN = 0;// disable 2 mhz rc gen*/

       //32 mhz without calibration
/*          OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
          while(RC32MRDY_bit == 0);
          CPU_CCP = 0xD8;
          CLK_CTRL = 1;*/
          
	RTC32_CTRL = 0x01;

// clkout on port C pin 7
	PORTC_DIR = 0xFF;
//	PORTC_DIR.B7 = 1;
	PORTCFG_CLKEVOUT.CLKOUT0 = 1;
	PORTCFG_CLKEVOUT.RTCOUT = 1;

     while (1);
}