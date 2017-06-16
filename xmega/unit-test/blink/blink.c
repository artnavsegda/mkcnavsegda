void main()
{
	PORTD_DIRSET.B4 = 1;
        while (1)
        {
                PORTD_OUTTGL.B4 = 1;
                delay_ms(1000);
        }
}
