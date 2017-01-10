#include "prototype.h"
#include "ramonitor.h"
#include "modbus.h"
#include "sequencer.h"
#include "ports.h"
#include "adc.h"

extern unsigned int result;

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

float standard_concentration = 1.0;
int coefficent;
int zerolevelavg;

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
        //if (mystate->currentmode == TOTALMERCURY) table[TOTALMERCURYROW] = calculatecell(oversample(BSWAP_16(result), zerolevelavg, celllevelavg, celltempavg, c_twentie_five, kfactor)); // TODO: not working until averaging code and constant aquring
        splitfloat(&table[MONITORFLOWLOW], &table[MONITORFLOWHIGH], calculateflow(ADC_Voltage(ADCA_Read(2))));
        splitfloat(&table[VACUUMLOW],&table[VACUUMHIGH], calculatepressure(ADC_Voltage(ADCA_Read(4))));
        splitfloat(&table[DILUTIONPRESSURELOW],&table[DILUTIONPRESSUREHIGH], calculatepressure(ADC_Voltage(ADCA_Read(5))));
        splitfloat(&table[BYPASSPRESSURELOW],&table[BYPASSPRESSUREHIGH], calculatepressure(ADC_Voltage(ADCA_Read(6))));
        splitfloat(&table[TEMPERATUREOFSPECTROMETERLOW],&table[TEMPERATUREOFSPECTROMETERHIGH], (ADC_Voltage(ADCA_Read(3))-0.5)*100);
        splitfloat(&table[CODEOFACURRENTMODELOW],&table[CODEOFACURRENTMODEHIGH], (float)currentmode);
        splitfloat(&table[ERRORSANDWARNINGSLOW],&table[ERRORSANDWARNINGSHIGH], (float)statusword);
        splitfloat(&table[TOTALMERCURYCOEFFICENTLOW],&table[TOTALMERCURYCOEFFICENTHIGH], standard_concentration/(float)((long)coefficent-(long)zerolevelavg));
        
        table[2]++;
        table[3] = BSWAP_16(result);
        for (i = 0; i<8;i++)
                table[4+i] = ADCA_Read(i);
        for (i = 0; i<8;i++)
                table[12+i] = ADCB_Read(i);

}

int GetStatus(void)
{
        int genstatus = 0;
        if (ADC_Voltage(ADCA_Read(0)) < 1.0) genstatus |= LOW_LIGHT;
        if (ADC_Voltage(ADCA_Read(2)) < 0.0) genstatus |= LOW_FLOW;
        /*if (SERVO_4_RIGHT_IN)        genstatus |= CONVERTER;
        if (SERVO_2_RIGHT_IN)        genstatus |= WATLOW1;
        if (SERVO_2_LEFT_IN) genstatus |= WATLOW2;
        if (SERVO_3_RIGHT_IN)        genstatus |= WATLOW3;
        if (SERVO_3_LEFT_IN) genstatus |= WATLOW4;*/
        return genstatus;
}