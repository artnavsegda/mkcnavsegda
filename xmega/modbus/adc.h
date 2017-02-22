#define ADCB_PMT_Current 0
#define ADCB_PMT_Voltage 1
#define ADCB_Flow 2
#define ADCB_Cell 3
#define ADCA_Bypass 6
#define ADCA_Dilution 5
#define ADCA_Vacuum 4

float ADC_Voltage(int adcvalue);
float TMP_Celsius(float voltage);