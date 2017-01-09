#include "prototype.h"
#include "spi.h"

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

void Prototype_Init(void)
{
        Sysclk_Init();
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        SPIC_Init();
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
}

unsigned char tick = 0;

void main() {
        unsigned int result;
	Prototype_Init();
	
	while(1)
	{
	        SPI_Ethernet_doPacket();
	        if (ad7707_drdy == 0)
	        {
	                SPIC_Write(0x38);
	                SPIC_Read_Bytes((unsigned char *)&result,2);
         	}
         	if (tick == 1)
                {
                        LED0_Toggle = 1;
                        table[2]++;
                        table[3] = BSWAP_16(result);
                        for (i = 0; i<8;i++)
                                table[4+i] = ADCA_Read(i);
                        for (i = 0; i<8;i++)
                                table[12+i] = ADCB_Read(i);
                        tick = 0;
                }
	 }
 }