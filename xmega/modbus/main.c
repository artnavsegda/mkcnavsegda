#include  <__EthEnc28j60.h>
#include "ad7705.h"
#include "ports.h"
#include "bswap.h"
#include "modbus.h"
#include "average.h"
#include "i2c.h"
#include "timer.h"

struct massive firststage;
struct massive secondstage;

void PrintHandler(char c)
{
        UART_Write(c);
}

int tick = 0;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        tick = 1;
}

void Ports_Init(void)
{
        LED0_Direction = 1;
        LED2_Direction = 1;
        Expander_Init(PORTU1);
        CELL_LeftOut_Direction = 0;
        CELL_RightOut_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void main()
{
        int i;
        unsigned int result;
        Ports_Init();
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        AD7705_Init();
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        Entermode(STARTLEVEL);

        while (1)
        {
                SPI_Ethernet_doPacket();
                if (AD7707_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                        increment(&firststage,BSWAP_16(result));
                }
                if (tick == 1)
                {
                        tick = 0;
                        LED2_Toggle = 1;
                        table[2]++;
                        table[3] = BSWAP_16(result);
                        table[4] = oversample(&firststage,16)/16;
                        table[5] = oversample(&firststage,32)/32;
                        table[6] = oversample(&firststage,64)/64;
                        increment(&secondstage,oversample(&firststage,64)/64);
                        table[7] = oversample(&secondstage,8)/8;
                        table[8] = currentmode;
                        /*for (i = 0; i<8;i++)
                                table[4+i] = ADCA_Read(i);
                        for (i = 0; i<8;i++)
                                table[12+i] = ADCB_Read(i);
                        for (i = 0; i<16;i=i+2)
                                splitfloat(&table[20+i], &table[21+i], (float)(ADCA_Read(i/2)/(float)4096));
                        for (i = 0; i<16;i=i+2)
                                splitfloat(&table[36+i], &table[37+i], (float)(ADCB_Read(i/2)/(float)4096));*/
                        if (bctable[0] == 0)
                        {
                                CELL_LeftOut = 1;
                                CELL_RightOut = 0;
                        }
                        else
                        {
                                CELL_LeftOut = 0;
                                CELL_RightOut = 1;
                        }
                        timetoexitmode--;
                        if (timetoexitmode == 0)
                                Exitmode(currentmode);
                        Expander_Write_Port(PORTU1,PORTU1_OUT);
                        Expander_Write_Port(PORTU3,PORTU3_OUT);
                }
        }
}