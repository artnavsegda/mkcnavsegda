#include "ntp.h"

void ntp_recieve(struct ntpframestruct myframe);
{
	printf("NTP lvm: 0x%X\n",myframe->leapvermode);
	printf("NTP stratum: 0x%X\n",myframe->stratumlevel);
	printf("NTP poll: 0x%X\n",myframe->poll);
	printf("NTP precision: 0x%X\n",myframe->precision);
	printf("NTP root delay: %u\n",ntohl(myframe->rootdelay));
	printf("NTP drift rate: %u\n",ntohl(myframe->driftrate));
	printf("NTP reference id: 0x%X\n",myframe->referenceid);
	printf("NTP reference time: %lu\n",ntohl(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
	printf("NTP reference fraction: %u\n",ntohl(myframe->reference->timefraction));
	printf("NTP origin time: %lu\n",ntohl(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
	printf("NTP origin fraction: %u\n",ntohl(myframe->origin->timefraction));
	printf("NTP receive time: %lu\n",ntohl(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
	printf("NTP receive fraction: %u\n",ntohl(myframe->receive->timefraction));
	printf("NTP transmit time: %lu\n",ntohl(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
	printf("NTP transmit fraction: %u\n",ntohl(myframe->transmit->timefraction));
        return;
}