#include  <__EthEnc28j60.h>
#include "ad7705.h"
#include "ports.h"
#include "bswap.h"
#include "modbus.h"
#include "average.h"
#include "i2c.h"
#include "timer.h"
#include "adc.h"

struct massive firststage;
struct massive secondstage;
struct massive temperature_averaging_massive;

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

unsigned int result;
unsigned int coefficent = 0x17CC;
unsigned int zerolevelavg = 0x17CC;
unsigned int celllevelavg = 4000+0x17CC;
unsigned int celltempavg = 1670;

void Fill_Table(void)
{
        table[2]++;
        table[3] = currentmode;
        table[4] = timetoexitmode;
        table[5] = BSWAP_16(result)-0x17CC;
        table[6] = (oversample(&firststage,64)/64)-0x17CC;
        table[7] = ADCB_Get_Sample(ADCB_Cell);
        splitfloat(&table[8],&table[9],ADC_Voltage(ADCB_Get_Sample(ADCB_Cell)));
        splitfloat(&table[10],&table[11],TMP_Celsius(ADC_Voltage(ADCB_Get_Sample(ADCB_Cell))));
        table[12] = coefficent-0x17CC;
        table[13] = zerolevelavg-0x17CC;
        table[14] = celllevelavg-0x17CC;
        table[15] = celltempavg;
        splitfloat(&table[16],&table[17], ADC_Voltage(celltempavg));
        splitfloat(&table[18],&table[19], TMP_Celsius(ADC_Voltage(celltempavg)));
        table[20] = (int)PORTU1_IN;
        table[21] = (int)PORTU1_OUT;
        table[22] = (int)PORTU2_IN;
        table[23] = (int)PORTU2_OUT;
        table[24] = (int)PORTU3_IN;
        table[25] = (int)PORTU3_OUT;
}

void main()
{
        int i;
        Ports_Init();
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        AD7705_Init();
        ADCA_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
        ADCB_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
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
                        increment(&secondstage,oversample(&firststage,64)/64);
                        increment(&temperature_averaging_massive,ADCB_Get_Sample(ADCB_Cell));
                        timetoexitmode--;
                        if (timetoexitmode == 0)
                                Exitmode(currentmode);
                                
                        PORTU1_IN = Expander_Read_Port(PORTU1);
                        PORTU2_IN = Expander_Read_Port(PORTU2);
                        PORTU3_IN = Expander_Read_Port(PORTU3);
                                
                        Fill_Table();
                        
                        Expander_Write_Port(PORTU1,PORTU1_OUT);
                        Expander_Write_Port(PORTU2,PORTU2_OUT);
                        Expander_Write_Port(PORTU3,PORTU3_OUT);
                }
        }
}