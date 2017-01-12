sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        LED0_Toggle = 1;
}

void main()
{
        LED0_Direction = 1;
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        
        while(1);
}