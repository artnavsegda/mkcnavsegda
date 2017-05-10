#include <__EthEnc28j60.h>
#include <__Lib_MmcFat16.h>
#include "ad7705.h"
#include "ports.h"
#include "bswap.h"
#include "modbus.h"
#include "average.h"
#include "i2c.h"
#include "timer.h"
#include "adc.h"
#include "ad7705.h"
#include "global.h"
#include "settings.h"
#include "rtc.h"
#include "ntp.h"
#include "log.h"

extern float rolidol;

void PrintHandler(char c)
{
        UART_Write(c);
}

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF
{
        tick = 1;
        asm wdr;
        SPI_Ethernet_userTimerSec++;
}

void Timer1Overflow_ISR() org IVT_ADDR_TCD0_OVF
{
        increment(&burnstage,BSWAP_16(result));
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
        Calibration_Valve_Direction = 0;
        Purge_Valve_Direction = 0;
        Expander_Set_DirectionPort(PORTU1,PORTU1_DIR);
        Expander_Init(PORTU2);
        Expander_Init(PORTU3);
        IgnitionDirection = 0;
        Expander_Set_DirectionPort(PORTU3,PORTU3_DIR);
}

void Fill_Table(void)
{
        char lowlight = ADCB_Get_Sample(ADCB_PMT_Current) < atoi(getmyopt("lowlight"));
        char lowflow = ADCB_Get_Sample(ADCB_Flow) < atoi(getmyopt("lowflow"));
        bctable[0] = !(lowlight | lowflow);
        bctable[1] = !(PORTU3_IN.B6 | PORTU2_IN.B7 | PORTU1_IN.B1 | PORTU2_IN.B3 | PORTU2_IN.B4);
        splitfloat(&table[8],&table[9], (float)currentmode);
        if (currentmode == TOTALMERCURY)
                splitfloat(&table[10],&table[11], (((float)(oversample(&firststage,64)/64.0-zerolevelavg)/(float)(celllevelavg-zerolevelavg))*rolidol));
        else
                splitfloat(&table[12],&table[13], (((float)(oversample(&firststage,64)/64.0-zerolevelavg)/(float)(celllevelavg-zerolevelavg))*rolidol));
        splitfloat(&table[14],&table[15], ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)));
        splitfloat(&table[16],&table[17], ADC_Voltage(ADCA_Get_Sample(ADCA_Vacuum)));
        splitfloat(&table[18],&table[19], ADC_Voltage(ADCA_Get_Sample(ADCA_Dilution)));
        splitfloat(&table[20],&table[21], ADC_Voltage(ADCA_Get_Sample(ADCA_Bypass)));
        splitfloat(&table[22],&table[23], TMP_Celsius(ADC_Voltage(ADCB_Get_Sample(ADCB_Cell))));
        splitfloat(&table[28],&table[29], (float)((lowlight<<1)|(lowflow<<2)|(PORTU3_IN.B6<<3)|(PORTU2_IN.B7<<4)|(PORTU1_IN.B1<<5)|(PORTU2_IN.B3<<6)|(PORTU2_IN.B4<<7)));
        splitfloat(&table[30],&table[31], rolidol/(float)(celllevelavg-zerolevelavg));
        splitfloat(&table[32],&table[33], (float)ADCB_Get_Sample(ADCB_PMT_Current));
        splitfloat(&table[40],&table[41], (float)ADCB_Get_Sample(ADCB_PMT_Voltage));
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
        unsigned long filesize, no_bytes;
        int i;
        MM_Init();
        Sysclk_Init();
        Ports_Init();
        UARTC0_Init(9600);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        AD7705_Init();
        ADCA_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
        ADCA_PRESCALER.B2 = 1; //div64
        //ADCA_PRESCALER = 7; //div512
        ADCB_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF_VCC);
        ADCB_PRESCALER.B2 = 1; //div64
        //ADCB_PRESCALER = 7; //div512
        sd_init = Mmc_Fat_Init();
        if (sd_init == 0)
        {
                if (Mmc_Fat_Assign("SETTINGS.TXT",0x80) == 1)
                {
                        Mmc_Fat_Reset(&filesize);
                        no_bytes = Mmc_Fat_ReadN(settings, filesize);
                        settings[no_bytes] = '\0'; //this is important every time
                }
                else
                        Mmc_Fat_Write(settings,strlen(settings));
                Mmc_Fat_Close();
        }
        else if (sd_init == 1)
        {
                Mmc_Fat_QuickFormat("DIGITAL");
                Mmc_Fat_Assign("SETTINGS.TXT",0x80);
                Mmc_Fat_Write(settings,strlen(settings));
                Mmc_Fat_Close();
                sd_init = 0;
        }
        makeopt();
        SPI_Ethernet_Init(getmymac(getmyopt("mac")), getmyip(getmyopt("ip")), 1);
        SPI_Ethernet_writeReg(EIE, 0xDB);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        Timer_Init(&TCD0, 100000);
        Timer_Interrupt_Enable(&TCD0);
        PMIC_CTRL.HILVLEN = 1;
        I_bit = 1;
        status_vbat();
        Log_Init();
        ntp_send();
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
                        //PrintOut(PrintHandler, "RTC time is %lu\r\n", rtc_get_time());
                }
        }
}