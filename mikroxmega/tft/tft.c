// TFT display connections
char TFT_16bit_DataPort_Lo at PORTC_OUT;
char TFT_16bit_DataPort_Hi at PORTA_OUT;
sbit TFT_16bit_WR at PORTB_OUT.B3;
sbit TFT_16bit_RD at PORTB_OUT.B2;
sbit TFT_16bit_CS at PORTB_OUT.B1;
sbit TFT_16bit_RS at PORTB_OUT.B0;
sbit TFT_16bit_RST at PORTB_OUT.B4;

char TFT_16bit_DataPort_Lo_Direction at PORTC_DIR;
char TFT_16bit_DataPort_Hi_Direction at PORTA_DIR;
sbit TFT_16bit_WR_Direction at PORTB_DIR.B3;
sbit TFT_16bit_RD_Direction at PORTB_DIR.B2;
sbit TFT_16bit_CS_Direction at PORTB_DIR.B1;
sbit TFT_16bit_RS_Direction at PORTB_DIR.B0;
sbit TFT_16bit_RST_Direction at PORTB_DIR.B4;
// End of TFT display connections


void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main() {
        TFT_16bit_Rotate_180(1);
        TFT_Init_ILI9341_16bit(240, 320);

        TFT_16bit_Fill_Screen(CL_BLACK_16bit);
        //delay_ms(1000);

        TFT_16bit_Set_Font(TFT_16bit_defaultFont, CL_WHITE_16bit, FO_VERTICAL_16bit);

        TFT_16bit_Write_Text("TFT 16-bit Library DEMO, WELCOME !", 0, 319);

        while(1)
        {
                //TFT_16bit_Fill_Screen(CL_LIME_16bit);
                //delay_ms(1000);
                //TFT_16bit_Fill_Screen(CL_WHITE_16bit);
                //delay_ms(1000);

                //TFT_16bit_Write_Text("TFT 16-bit Library DEMO, WELCOME !", 0, 0);

        }
}
