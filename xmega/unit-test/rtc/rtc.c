unsigned long rtc_get_time(void)
{
        RTC32_SYNCCTRL.SYNCCNT = 1;
        while (RTC32_SYNCCTRL.SYNCCNT);
        return RTC32_CNT;
}

/*void rtc_set_time(unsigned long time)
{
        RTC32_CTRL = 0;
        while (SYNCBUSY_RTC32_SYNCCTRL_bit);
        RTC32_CNT = time;
        ENABLE_RTC32_CTRL_bit = 1;
}

void sysclk_init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}*/

void vbat_init(void)
{
        VBAT_CTRL.ACCEN = 1;
        CPU_CCP = 0xD8;
        RESET__VBAT_CTRL_bit = 1;
        XOSCFDEN_VBAT_CTRL_bit = 1;
        delay_ms(2);
        XOSCEN_VBAT_CTRL_bit = 1;
        while (XOSCRDY_VBAT_STATUS_bit == 0);
}

void rtc_init(void)
{
        //PR_PRGEN.RTC = 0;
        vbat_init();
        RTC32_CTRL = 0;
        while (SYNCBUSY_RTC32_SYNCCTRL_bit);
        RTC32_PER = 0xffffffff;
        RTC32_CNT = 0;
        while (SYNCBUSY_RTC32_SYNCCTRL_bit);
        RTC32_INTCTRL = 0;
        ENABLE_RTC32_CTRL_bit = 1;
        while (SYNCBUSY_RTC32_SYNCCTRL_bit);
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void status_vbat(void)
{
        PR_PRGEN.RTC = 0; //enable rtc(one?no?)
        if (VBAT_STATUS.BBPWR)
                PrintOut(PrintHandler, "VBAT has no power\n\r");
        else if (VBAT_STATUS.XOSCFAIL)
                PrintOut(PrintHandler, "RTC crystal oscillator failed\n\r");
        else if (VBAT_STATUS.BBPORF)
                PrintOut(PrintHandler, "VBAT had brownout while no power is present\n\r");
        else if (VBAT_STATUS.BBPORF)
                PrintOut(PrintHandler, "VBAT had power-on reset\n\r");
        else
        {
                PrintOut(PrintHandler, "VBAT OK\n\r");
                VBAT_CTRL.ACCEN = 1;
		return;
        }
        rtc_init();
}

void main()
{
        UARTC0_Init(9600);
        PrintOut(PrintHandler, "MCU started\r\n");
        status_vbat();
        while(1)
        {
                PrintOut(PrintHandler, "RTC time is %lu\r\n", rtc_get_time());
                delay_ms(1000);
        }
}