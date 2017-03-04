unsigned long rtc_get_time(void)
{
        RTC32_SYNCCTRL = SYNCCNT;
        while (SYNCCNT_bit & SYNCCNT)
        return RTC32_CNT;
}

void rtc_set_time(unsigned long time)
{
        RTC32_CTRL = 0;
        while (SYNCBUSY_RTC32_SYNCCTRL_bit)
        RTC32_CNT = time;
        ENABLE_RTC32_CTRL_bit = 1;
}

void main()
{
        PR_PRGEN &= ~RTC;
}