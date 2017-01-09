#include "prototype.h"
#include "ramonitor.h"
#include "modbus.h"
#include "sequencer.h"
#include "ports.h"
#include "adc.h"

extern unsigned int result;

//everything not relevant until float modbus code

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
       	table[MONITORFLOW] = calculateflow(ADC_Voltage(ADCA_Read[2]));
	table[VACUUM] = calculatepressure(ADC_Voltage(ADCA_Read[4]));
	table[DILUTIONPRESSURE] = calculatepressure(ADC_Voltage(ADCA_Read[5])));
	table[BYPASSPRESSURE] = calculatepressure(ADC_Voltage(ADCA_Read[6])));
	table[TEMPERATUREOFSPECTROMETER] = (ADC_Voltage(ADCA_Read[3])-0.5)*100);
	table[CODEOFACURRENTMODE] = currentmode;
	table[ERRORSANDWARNINGS] = statusword; // Errors and warnings
	table[TOTALMERCURYCOEFFICENT] = standard_concentration/(float)((long)coefficent-(long)zerolevelavg)); // Total mercury coefficent
	
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
	/*if (SERVO_4_RIGHT_IN)	genstatus |= CONVERTER;
	if (SERVO_2_RIGHT_IN)	genstatus |= WATLOW1;
	if (SERVO_2_LEFT_IN) genstatus |= WATLOW2;
	if (SERVO_3_RIGHT_IN)	genstatus |= WATLOW3;
	if (SERVO_3_LEFT_IN) genstatus |= WATLOW4;*/
	return genstatus;
}