//coil
#define STATUSOFSPECTROMETER 0
#define STATUSOFTHERMOCONTROLLERS 1
#define AVAILABILITYOFEXTERNALREQUEST 2
#define STATUSOFZEROTEST 3
#define STATUSOFCALIBRATION 4
//reg
#define ELEMENTALMERCURYROWLOW 24
#define ELEMENTALMERCURYROWHIGH 25
#define TOTALMERCURYROWLOW 10
#define TOTALMERCURYROWHIGH 11
#define OXIDIZEDMERCURYROWLOW 12
#define TOTALMERCURYROWHIGH 13
#define MONITORFLOWLOW 14
#define MONITORFLOWHIGH 15
#define VACUUMLOW 16
#define VACUUMHIGH 17
#define DILUTIONPRESSURELOW 18
#define DILUTIONPRESSUREHIGH 19
#define BYPASSPRESSURELOW 20
#define BYPASSPRESSUREHIGH 21
#define TEMPERATUREOFSPECTROMETERLOW 22
#define TEMPERATUREOFSPECTROMETERHIGH 23
#define CODEOFACURRENTMODELOW 8
#define CODEOFACURRENTMODEHIGH 9
#define ERRORSANDWARNINGSLOW 28
#define ERRORSANDWARNINGSHIGH 29
#define TOTALMERCURYCOEFFICENTLOW 30
#define TOTALMERCURYCOEFFICENTHIGH 31

#define REQUESTTOSTARTCALIBRATION 99
#define REQUESTTOSTARTZEROTEST 100
#define REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY 101
#define REQUESTTOSTARTPURGE 102
#define REQUESTTOENDPURGE 103

void RAmonitor(void);
int GetStatus(void);