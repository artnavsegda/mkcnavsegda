sfr sbit LED0 at PORTR_OUT.B0;
sfr sbit LED1 at PORTR_OUT.B1;
sfr sbit LED2 at PORTD_OUT.B4;
sfr sbit LED3 at PORTD_OUT.B5;

sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED1_Direction at PORTR_DIR.B1;
sfr sbit LED2_Direction at PORTD_DIR.B4;
sfr sbit LED3_Direction at PORTD_DIR.B5;

sfr sbit LCD_BACKLIGHT_ENABLE_PIN at PORTE_OUT.B4;

sfr sbit GPIO_PUSH_BUTTON_0 at PORTE_OUT.B5;
sfr sbit GPIO_PUSH_BUTTON_1 at PORTF_OUT.B1;
sfr sbit GPIO_PUSH_BUTTON_2 at PORTF_OUT.B2;

sfr sbit TWIC_SDA at PORTC_OUT.B0;
sfr sbit TWIC_SCL at PORTC_OUT.B1;
sfr sbit USARTC0_RXD at PORTC_OUT.B2;
sfr sbit USARTC0_TXD at PORTC_OUT.B3;
sfr sbit SPIC_SS at PORTC_OUT.B4;
sfr sbit SPIC_MOSI at PORTC_OUT.B5;
sfr sbit SPIC_MISO at PORTC_OUT.B6;
sfr sbit SPIC_SCK at PORTC_OUT.B7;

sfr sbit USARTD0_RXD at PORTD_OUT.B2;
sfr sbit USARTD0_TXD at PORTD_OUT.B3;

sfr sbit USARTE0_RXD at PORTE_OUT.B2;
sfr sbit USARTE0_TXD at PORTE_OUT.B3;

sfr sbit J1_PIN0 at PORTC_OUT.B0;
sfr sbit J1_PIN1 at PORTC_OUT.B1;
sfr sbit J1_PIN2 at PORTC_OUT.B2;
sfr sbit J1_PIN3 at PORTC_OUT.B3;
sfr sbit J1_PIN4 at PORTC_OUT.B4;
sfr sbit J1_PIN5 at PORTC_OUT.B5;
sfr sbit J1_PIN6 at PORTC_OUT.B6;
sfr sbit J1_PIN7 at PORTC_OUT.B7;

sfr sbit J2_PIN0 at PORTB_OUT.B0;
sfr sbit J2_PIN1 at PORTB_OUT.B1;
sfr sbit J2_PIN2 at PORTB_OUT.B2;
sfr sbit J2_PIN3 at PORTB_OUT.B3;
sfr sbit J2_PIN4 at PORTA_OUT.B4;
sfr sbit J2_PIN5 at PORTA_OUT.B5;
sfr sbit J2_PIN6 at PORTA_OUT.B6;
sfr sbit J2_PIN7 at PORTA_OUT.B7;

sfr sbit J3_PIN0 at PORTA_OUT.B0;
sfr sbit J3_PIN1 at PORTA_OUT.B1;
sfr sbit J3_PIN2 at PORTA_OUT.B2;
sfr sbit J3_PIN3 at PORTA_OUT.B3;
sfr sbit J3_PIN4 at PORTB_OUT.B4;
sfr sbit J3_PIN5 at PORTB_OUT.B5;
sfr sbit J3_PIN6 at PORTB_OUT.B6;
sfr sbit J3_PIN7 at PORTB_OUT.B7;

sfr sbit J4_PIN0 at PORTE_OUT.B0;
sfr sbit J4_PIN1 at PORTE_OUT.B1;
sfr sbit J4_PIN2 at PORTE_OUT.B2;
sfr sbit J4_PIN3 at PORTE_OUT.B3;
sfr sbit J4_PIN4 at PORTD_OUT.B0;
sfr sbit J4_PIN5 at PORTD_OUT.B3;
sfr sbit J4_PIN6 at PORTD_OUT.B2;
sfr sbit J4_PIN7 at PORTD_OUT.B1;

void board_init(void)
{
        LED0_Direction = 1;
        LED1_Direction = 1;
        LED2_Direction = 1;
        LED3_Direction = 1;

        PORTE_PIN5CTRL = 0x18; // gpio push button 0 pull up
        PORTF_PIN1CTRL = 0x18; // gpio push button 1 pull up
        PORTF_PIN1CTRL = 0x18; // gpio push button 2 pull up
        
#ifdef CONF_BOARD_ENABLE_USARTC0
        PORTC_DIR.B3 = 1;
        PORTC_OUT.B2 = 0;
#endif

#ifdef CONF_BOARD_ENABLE_USARTD0
        PORTD_DIR.B3 = 1;
        PORTD_OUT.B2 = 0;
#endif

#ifdef CONF_BOARD_ENABLE_USARTE0
        PORTE_DIR.B3 = 1;
        PORTE_OUT.B2 = 0;
#endif

}