void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void Sysclk_Init(void)
{
	OSC_CTRL = 0x02;
	while(RC32MRDY_bit == 0);
	CPU_CCP = 0xD8;
	CLK_CTRL = 1;
}

void main() {

}