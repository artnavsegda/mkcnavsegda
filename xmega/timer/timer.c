void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        PORTD_OUTTGL.B4 = 1;
}

void main()
{
	PORTD_DIRSET.B4 = 1;
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        PMIC_CTRL.HILVLEN = 1;
        asm sei;

        while (1);
}
