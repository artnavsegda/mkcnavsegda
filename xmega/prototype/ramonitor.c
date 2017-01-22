#include "prototype.h"
#include "ramonitor.h"
#include "modbus.h"
#include "sequencer.h"
#include "ports.h"
#include "adc.h"
#include "i2c.h"
#include "average.h"

extern unsigned int result;
float c_twentie_five = 1297.17;
float kfactor = 0.0082;
float standard_concentration = 1.0;
int coefficent;
int zerolevelavg;
int celllevelavg;
int celltempavg;
extern int zerostage;

void RAmonitor(void)
{
        int i;
        char lowlight = ADC_Voltage(ADCB_Get_Sample(ADCB_PMT_Voltage)) < 1.0;
        char lowflow = ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)) < 0.0;
        bctable[0] = lowlight | lowflow;
        bctable[1] = CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4;
        bctable[2] = (currentmode == TOTALMERCURY);
        bctable[3] = (currentmode == ZEROTEST || currentmode == ZERODELAY);
        bctable[4] = (currentmode == CALIBRATION || currentmode == PRECALIBRATIONDELAY || currentmode == POSTCALIBRATIONDELAY);
        if (currentmode == TOTALMERCURY) splitfloat(&table[10], &table[11], (((float)(zerostage-zerolevelavg)/(float)(celllevelavg-zerolevelavg))*(1297.17*exp(0.0082*(TMP_Celsius(ADC_Voltage(celltempavg))-25)))));
        splitfloat(&table[14], &table[15], ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)));
        splitfloat(&table[16],&table[17], ADC_Voltage(ADCA_Get_Sample(ADCA_Vacuum)));
        splitfloat(&table[18],&table[19], ADC_Voltage(ADCA_Get_Sample(ADCA_Dilution)));
        splitfloat(&table[20],&table[21], ADC_Voltage(ADCA_Get_Sample(ADCA_Bypass)));
        splitfloat(&table[22],&table[23], TMP_Celsius(ADC_Voltage(ADCB_Get_Sample(ADCB_Cell))));
        splitfloat(&table[8],&table[9], (float)currentmode);
        splitfloat(&table[28],&table[29], (float)((lowlight<<1)|(lowflow<<2)|(CONVERTER<<3)|(WATLOW1<<4)|(WATLOW2<<5)|(WATLOW3<<6)|(WATLOW4<<7)));
        splitfloat(&table[30],&table[31], standard_concentration/(float)((long)coefficent-(long)zerolevelavg));
}