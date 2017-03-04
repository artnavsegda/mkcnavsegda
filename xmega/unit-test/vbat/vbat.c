void PrintHandler(char c)
{
	UARTC0_Write(c);
}

void main()
{
        PR_PRGEN &= ~RTC; //enable rtc
        PrintOut(PrintHandler, "VBAT power %d\r\n", VBAT_STATUS.BBPWR);
        
}