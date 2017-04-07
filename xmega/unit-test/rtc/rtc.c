unsigned long rtc_get_time(void)
{
        RTC32_SYNCCTRL = SYNCCNT;
        while (SYNCCNT_bit & SYNCCNT);
        return RTC32_CNT;
}

void rtc_set_time(unsigned long time)
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
}

void vbat_init(void)
{
        VBAT_CTRL.ACCEN = 1;
        CPU_CCP = 0xD8;
        RESET__VBAT_CTRL_bit = 1;
        XOSCFDEN_VBAT_CTRL_bit = 1;
	delay_ms(200);
	XOSCEN_VBAT_CTRL_bit = 1;
	while (XOSCRDY_VBAT_STATUS_bit == 0);
}

void rtc_init(void)
{
        PR_PRGEN &= ~RTC;
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