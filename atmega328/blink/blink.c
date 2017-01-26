void main()
{
        DDB5_bit = 1;
        while (1)
        {
        	PORTB5_bit = 1;
        	delay_ms(1000);
        	PORTB5_bit = 0;
        	delay_ms(1000);
	}
}