void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void init_vbat(void)
{
        PR_PRGEN.RTC = 1; //enable rtc(maybe zero ? idunno, explain)
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
        }
}

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

void main()
{
        UARTC0_Init(9600);
        PrintOut(PrintHandler, "MCU started\r\n");
        init_vbat();
        //PrintOut(PrintHandler, "VBAT power %d\r\n", VBAT_STATUS.BBPWR);
        while(1);
}