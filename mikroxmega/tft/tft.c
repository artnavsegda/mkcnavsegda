// TFT display connections
char TFT_16bit_DataPort_Lo at PORTJ_OUT;
char TFT_16bit_DataPort_Hi at PORTK_OUT;
sbit TFT_16bit_WR at PORTH_OUT.B5;
sbit TFT_16bit_RD at PORTH_OUT.B6;
sbit TFT_16bit_CS at PORTB_OUT.B3;
sbit TFT_16bit_RS at PORTB_OUT.B4;
sbit TFT_16bit_RST at PORTH_OUT.B7;

char TFT_16bit_DataPort_Lo_Direction at PORTJ_DIR;
char TFT_16bit_DataPort_Hi_Direction at PORTK_DIR;
sbit TFT_16bit_WR_Direction at PORTH_DIR.B5;
sbit TFT_16bit_RD_Direction at PORTH_DIR.B6;
sbit TFT_16bit_CS_Direction at PORTH_DIR.B3;
sbit TFT_16bit_RS_Direction at PORTH_DIR.B4;
sbit TFT_16bit_RST_Direction at PORTH_DIR.B7;
// End of TFT display connections


void SysClk_Init() {
        // ----- External Clock selection procedure!!!
        OSC_XOSCCTRL = 0x40;   // freq. range 2 - 9MHz; External Clock
        OSC_CTRL |= 0x08;      // enable external oscillator
        while(XOSCRDY_bit == 0);
        CPU_CCP = 0xD8;  // The CCP register must be written with the correct signature to enable change of the protected
                         // I/O register or execution of the protected instruction for a maximum of 4 CPU instruction cycles.
        CLK_CTRL = 3;    // External oscillator
        // ---------------------------------
}

void main() {
	SysClk_Init();
        TFT_16bit_Rotate_180(1);
        TFT_Init_ILI9341_16bit(240, 320);

        TFT_16bit_Fill_Screen(CL_BLACK_16bit);
        //delay_ms(1000);

        TFT_16bit_Set_Font(TFT_16bit_defaultFont, CL_WHITE_16bit, FO_VERTICAL_16bit);

        TFT_16bit_Write_Text("TFT 16-bit Library DEMO, WELCOME !", 0, 319);

        while(1)
        {
                TFT_16bit_Fill_Screen(CL_LIME_16bit);
                delay_ms(1000);
                TFT_16bit_Fill_Screen(CL_WHITE_16bit);
                delay_ms(1000);

                TFT_16bit_Write_Text("TFT 16-bit Library DEMO, WELCOME !", 0, 0);

        }
}