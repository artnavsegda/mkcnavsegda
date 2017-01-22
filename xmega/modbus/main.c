#include  <__EthEnc28j60.h>
#include "ad7705.h"
#include "ports.h"
#include "bswap.h"
#include "modbus.h"
#include "average.h"
#include "i2c.h"
#include "timer.h"
#include "adc.h"
#include "ad7705.h"

#define ALLOK 0
#define NO_DATA 1
#define LOW_LIGHT 2
#define LOW_FLOW 4
#define CONVERTER 8
#define WATLOW1 16
#define WATLOW2 32
#define WATLOW3 64
#define WATLOW4 128

struct massive firststage;
struct massive secondstage;
struct massive temperature_averaging_massive;
unsigned int result;
int zerostage;
int coefficent = ADCZERO;
int zerolevelavg = ADCZERO;
int celllevelavg = 4000+ADCZERO;
int celltempavg = 1670;
int tick = 0;

void PrintHandler(char c)
{
        UART_Write(c);
}

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
        Calibration_Valve_Direction = 0;
        Zero_Valve_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void Fill_Table(void)
{
        char lowlight = ADC_Voltage(ADCB_Get_Sample(ADCB_PMT_Voltage)) < 1.0;
        char lowflow = ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)) < 0.0;
        bctable[0] = lowlight | lowflow;
        bctable[1] = PORTU3_IN.B6 | PORTU2_IN.B7 | PORTU1_IN.B1 | PORTU2_IN.B3 | PORTU2_IN.B4;
        splitfloat(&table[8],&table[9], (float)currentmode);
        splitfloat(&table[10],&table[11], (((float)(zerostage-zerolevelavg)/(float)(celllevelavg-zerolevelavg))*(1297.17*exp(0.0082*(TMP_Celsius(ADC_Voltage(celltempavg))-25)))));
        splitfloat(&table[14],&table[15], ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)));
        splitfloat(&table[16],&table[17], ADC_Voltage(ADCA_Get_Sample(ADCA_Vacuum)));
        splitfloat(&table[18],&table[19], ADC_Voltage(ADCA_Get_Sample(ADCA_Dilution)));
        splitfloat(&table[20],&table[21], ADC_Voltage(ADCA_Get_Sample(ADCA_Bypass)));
        splitfloat(&table[22],&table[23], TMP_Celsius(ADC_Voltage(ADCB_Get_Sample(ADCB_Cell))));
        splitfloat(&table[28],&table[29], (float)((lowlight<<1)|(lowflow<<2)|(PORTU3_IN.B6<<3)|(PORTU2_IN.B7<<4)|(PORTU1_IN.B1<<5)|(PORTU2_IN.B3<<6)|(PORTU2_IN.B4<<7)));
        splitfloat(&table[30],&table[31], (1297.17*exp(0.0082*(TMP_Celsius(ADC_Voltage(celltempavg))-25)))/(float)(celllevelavg-zerolevelavg));
}

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main()
{
        int i;
        Sysclk_Init();
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
                        zerostage = oversample(&firststage,64)/64;
                        increment(&secondstage,zerostage);
                        increment(&temperature_averaging_massive,ADCB_Get_Sample(ADCB_Cell));
                        timetoexitmode--;
                        if (timetoexitmode == 0)
                                Exitmode(currentmode);
                        Operatemode();
                        Expander_Read_All();
                        Fill_Table();
                        Expander_Write_All();
                }
        }
}