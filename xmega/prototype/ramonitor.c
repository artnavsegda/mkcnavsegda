#include "prototype.h"
#include "ramonitor.h"
#include "modbus.h"
#include "sequencer.h"
#include "ports.h"
#include "adc.h"
#include "i2c.h"
#include "average.h"

extern unsigned int result;

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor)
{
        return (
                        (long) averaged - (long) zerolevelavg
        ) / (float) (
                (long) celllevelavg - (long) zerolevelavg
        ) * (
                c_twentie_five * exp (
                        kfactor * (
                                (
                                        (
                                                (
                                                        (
                                                                celltempavg - 180 // ADC zero level
                                                        ) * (
                                                                (
                                                                        3.3 / 1.6 // Voltage reference
                                                                ) / 4095 // ADC resolution
                                                        )
                                                ) - 0.5
                                        ) * 100.0 // temperature in Celsius
                                ) - 25.0
                        )
                )
        );
}

float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration)
{
        return (
                (long) averaged - (long) zerolevelavg
        ) / (float) (
                (long) coefficent - (long) zerolevelavg
        ) * standard_concentration;
}

#define FLOW_SENSOR_SPAN 10
#define EXPECTED_FLOW_SENSOR_VOLTAGE 9.0
#define RESISTOR_DIVIDER 0.319
#define R2_RESISTOROHM 3.3
#define R5_RESISTOROHM 10.0

float calculateflow(float voltage)
{
        return (
        (
                (
                        voltage / (
                                R2_RESISTOROHM/(
                                        R5_RESISTOROHM+R2_RESISTOROHM
                                )
                        )
                ) / EXPECTED_FLOW_SENSOR_VOLTAGE
        ) - 0.1
        )*(
                FLOW_SENSOR_SPAN / 0.4
        );
}

float calculatepressure(float voltage)
{
        return (voltage-0.4)*12;
}

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
        int statusword;
        int i;
        statusword = GetStatus();
        bctable[0] = !(statusword & (LOW_LIGHT|LOW_FLOW));
        bctable[1] = !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4));
        bctable[2] = (currentmode == TOTALMERCURY);
        bctable[3] = (currentmode == ZEROTEST || currentmode == ZERODELAY);
        bctable[4] = (currentmode == CALIBRATION || currentmode == PRECALIBRATIONDELAY || currentmode == POSTCALIBRATIONDELAY);
        if (currentmode == ELEMENTALMERCURY) splitfloat(&table[24],&table[25], calculatecalibration(BSWAP_16(result), zerolevelavg, coefficent, standard_concentration));
        if (currentmode == TOTALMERCURY) splitfloat(&table[10], &table[11], (((float)(zerostage-zerolevelavg)/(float)(celllevelavg-zerolevelavg))*(1297.17*exp(0.0082*(TMP_Celsius(ADC_Voltage(celltempavg))-25)))));
        splitfloat(&table[14], &table[15], calculateflow(ADC_Voltage(ADCB_Get_Sample(ADCB_Flow))));
        splitfloat(&table[16],&table[17], calculatepressure(ADC_Voltage(ADCA_Get_Sample(ADCA_Vacuum))));
        splitfloat(&table[18],&table[19], calculatepressure(ADC_Voltage(ADCA_Get_Sample(ADCA_Dilution))));
        splitfloat(&table[20],&table[21], calculatepressure(ADC_Voltage(ADCA_Get_Sample(ADCA_Bypass))));
        splitfloat(&table[22],&table[23], (ADC_Voltage(ADCB_Get_Sample(ADCB_Cell))-0.5)*100);
        splitfloat(&table[8],&table[9], (float)currentmode);
        splitfloat(&table[28],&table[29], (float)statusword);
        splitfloat(&table[30],&table[31], standard_concentration/(float)((long)coefficent-(long)zerolevelavg));
        
        if (currentmode == TOTALMERCURY || currentmode == PURGE)
        {
                if (bctable[99] == 1)
                {
                        bctable[99] == 0;
                        Entermode(PRECALIBRATIONDELAY);
                }
                if (bctable[100] == 1)
                {
                        bctable[100] == 0;
                        Entermode(ZERODELAY);
                }
                if (bctable[101] == 1)
                {
                        bctable[101] == 0;
                        Entermode(ELEMENTALMERCURYDELAY);
                }
                if (bctable[102] == 1)
                {
                        bctable[102] == 0;
                        Entermode(PURGE);
                }
                if (bctable[103] == 1)
                {
                        bctable[103] == 0;
                        Exitmode(PURGE);
                }
        }
}

int GetStatus(void)
{
        int genstatus = 0;
        if (ADC_Voltage(ADCB_Get_Sample(ADCB_PMT_Voltage)) < 1.0) genstatus |= LOW_LIGHT;
        if (ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)) < 0.0) genstatus |= LOW_FLOW;
        if (SERVO_4_RIGHT_IN)        genstatus |= CONVERTER;
        if (SERVO_2_RIGHT_IN)        genstatus |= WATLOW1;
        if (SERVO_2_LEFT_IN) genstatus |= WATLOW2;
        if (SERVO_3_RIGHT_IN)        genstatus |= WATLOW3;
        if (SERVO_3_LEFT_IN) genstatus |= WATLOW4;
        return genstatus;
}