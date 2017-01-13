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

float c_twentie_five = 25.0;
float kfactor = 37.0;
float standard_concentration = 1.0;
int coefficent;
int zerolevelavg;
int celllevelavg;
int celltempavg;

void RAmonitor(void)
{
        int statusword;
        int i;
        statusword = GetStatus();
        bctable[STATUSOFSPECTROMETER] = !(statusword & (LOW_LIGHT|LOW_FLOW));
        bctable[STATUSOFTHERMOCONTROLLERS] = !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4));
        bctable[AVAILABILITYOFEXTERNALREQUEST] = (currentmode == TOTALMERCURY);
        bctable[STATUSOFZEROTEST] = (currentmode == ZEROTEST || currentmode == ZERODELAY);
        bctable[STATUSOFCALIBRATION] = (currentmode == CALIBRATION || currentmode == PRECALIBRATIONDELAY || currentmode == POSTCALIBRATIONDELAY);
        //if (currentmode == ELEMENTALMERCURY) table[ELEMENTALMERCURYROW] = calculatecalibration(BSWAP_16(result), zerolevelavg, coefficent, standard_concentration)); // TODO: not working until averaging code
        if (currentmode == TOTALMERCURY) splitfloat(&table[TOTALMERCURYROWLOW], &table[TOTALMERCURYROWHIGH], calculatecell(oversample(&ad7705_averaging_massive,32)/32, zerolevelavg, celllevelavg, celltempavg, c_twentie_five, kfactor));
        splitfloat(&table[MONITORFLOWLOW], &table[MONITORFLOWHIGH], calculateflow(ADC_Voltage(ADCB_Read(ADCB_Flow))));
        splitfloat(&table[VACUUMLOW],&table[VACUUMHIGH], calculatepressure(ADC_Voltage(ADCA_Read(ADCA_Vacuum))));
        splitfloat(&table[DILUTIONPRESSURELOW],&table[DILUTIONPRESSUREHIGH], calculatepressure(ADC_Voltage(ADCA_Read(ADCA_Dilution))));
        splitfloat(&table[BYPASSPRESSURELOW],&table[BYPASSPRESSUREHIGH], calculatepressure(ADC_Voltage(ADCA_Read(ADCA_Bypass))));
        splitfloat(&table[TEMPERATUREOFSPECTROMETERLOW],&table[TEMPERATUREOFSPECTROMETERHIGH], (ADC_Voltage(ADCB_Read(ADCB_Cell))-0.5)*100);
        splitfloat(&table[CODEOFACURRENTMODELOW],&table[CODEOFACURRENTMODEHIGH], (float)currentmode);
        splitfloat(&table[ERRORSANDWARNINGSLOW],&table[ERRORSANDWARNINGSHIGH], (float)statusword);
        splitfloat(&table[TOTALMERCURYCOEFFICENTLOW],&table[TOTALMERCURYCOEFFICENTHIGH], standard_concentration/(float)((long)coefficent-(long)zerolevelavg));
        
        if (currentmode == TOTALMERCURY || currentmode == PURGE)
        {
                if (bctable[REQUESTTOSTARTCALIBRATION] == 1)
                {
                        bctable[REQUESTTOSTARTCALIBRATION] == 0;
                        Entermode(PRECALIBRATIONDELAY);
		}
                if (bctable[REQUESTTOSTARTZEROTEST] == 1)
                {
                        bctable[REQUESTTOSTARTZEROTEST] == 0;
                        Entermode(ZERODELAY);
		}
                if (bctable[REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY] == 1)
                {
                        bctable[REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY] == 0;
                        Entermode(ELEMENTALMERCURYDELAY);
		}
                if (bctable[REQUESTTOSTARTPURGE] == 1)
                {
                        bctable[REQUESTTOSTARTPURGE] == 0;
                        Entermode(PURGE);
		}
                if (bctable[REQUESTTOENDPURGE] == 1)
                {
                        bctable[REQUESTTOENDPURGE] == 0;
                        Exitmode(PURGE);
		}
	}
}

int GetStatus(void)
{
        int genstatus = 0;
        if (ADC_Voltage(ADCA_Read(0)) < 1.0) genstatus |= LOW_LIGHT;
        if (ADC_Voltage(ADCA_Read(2)) < 0.0) genstatus |= LOW_FLOW;
        PORTU1_IN = Expander_Read_Port(PORTU1);
        PORTU2_IN = Expander_Read_Port(PORTU2);
        PORTU3_IN = Expander_Read_Port(PORTU3);

        if (SERVO_4_RIGHT_IN)        genstatus |= CONVERTER;
        if (SERVO_2_RIGHT_IN)        genstatus |= WATLOW1;
        if (SERVO_2_LEFT_IN) genstatus |= WATLOW2;
        if (SERVO_3_RIGHT_IN)        genstatus |= WATLOW3;
        if (SERVO_3_LEFT_IN) genstatus |= WATLOW4;
        return genstatus;
}
