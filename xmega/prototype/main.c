#include "prototype.h"
#include "ports.h"
#include "spi.h"
#include "ramonitor.h"
#include "sequencer.h"
#include "average.h"
#include "i2c.h"
#include "global.h"

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
        PMIC_CTRL = 4;
        CPU_SREG.B7 = 1;
        ADCA_PRESCALER.B2 = 1;
        ADCB_PRESCALER.B2 = 1;
}

void Port_Init(void)
{
        LED0_Direction = 1;
        LED2_Direction = 1;
        Expander_Init(PORTU1);
        CELL_LeftOut_Direction = 0;
        CELL_RightOut_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU3);
        Expander_Set_DirectionPort(PORTU2,PORTU2_DIR);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Elemental_Valve_Direction = 0;
        Calibration_Valve_Direction = 0;
        Zero_Valve_Direction = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void Prototype_Init(void)
{
        Sysclk_Init();
        Port_Init();
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        AD7705_Init();
        ADCA_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
        ADCB_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
        SPIC_Init();
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        Entermode(STARTLEVEL);
}

unsigned char tick = 0;
unsigned int result;
int zerostage;

void main() {

        Prototype_Init();
        while(1)
        {
                SPI_Ethernet_doPacket();
                if (AD7707_DRDY == 0)
                {
                        LED2_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                        increment(&ad7705_averaging_massive,BSWAP_16(result));
                }
                if (tick == 1)
                {
                        tick = 0;
                        LED2_Toggle = 1;
                        Average();
                        Sequencer();
                        Expander_Read_All();
                        RAmonitor();
                        Operatemode();
                        Expander_Write_All();
                }
        }
}