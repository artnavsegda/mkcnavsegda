#include "ntp.h"

void PrintHandler(char c)
{
        UART1_Write(c);
}

void ntp_recieve(struct ntpframestruct *myframe)
{
        PrintOut(PrintHandler, "NTP lvm: 0x%X\n",myframe->leapvermode);
        PrintOut(PrintHandler, "NTP stratum: 0x%X\n",myframe->stratumlevel);
        PrintOut(PrintHandler, "NTP poll: 0x%X\n",myframe->poll);
        PrintOut(PrintHandler, "NTP precision: 0x%X\n",myframe->precision);
        PrintOut(PrintHandler, "NTP root delay: %u\n",myframe->rootdelay);
        PrintOut(PrintHandler, "NTP drift rate: %u\n",myframe->driftrate);
        PrintOut(PrintHandler, "NTP reference id: 0x%X\n",myframe->referenceid);
        PrintOut(PrintHandler, "NTP reference time: %lu\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP reference fraction: %u\n",myframe->reference->timefraction);
        PrintOut(PrintHandler, "NTP origin time: %lu\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP origin fraction: %u\n",myframe->origin->timefraction);
        PrintOut(PrintHandler, "NTP receive time: %lu\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP receive fraction: %u\n",myframe->receive->timefraction);
        PrintOut(PrintHandler, "NTP transmit time: %lu\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP transmit fraction: %u\n",myframe->transmit->timefraction);
        return;
}

void ntp_send(void)
{
        unsigned char IpAddr[4]  = {192, 168,   1,  1 };  // remote IP address
        struct ntpframestruct myframe;
        myframe.leapvermode = 0x11;
        SPI_Ethernet_sendUDP(IpAddr, 123, 123, (unsigned char *)&myframe, 28);
}