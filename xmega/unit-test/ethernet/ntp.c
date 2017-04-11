#include "ntp.h"

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void ntp_recieve(struct ntpframestruct *myframe)
{
        PrintOut(PrintHandler, "NTP lvm: 0x%X\r\n",myframe->leapvermode);
        PrintOut(PrintHandler, "NTP stratum: 0x%X\r\n",myframe->stratumlevel);
        PrintOut(PrintHandler, "NTP poll: 0x%X\r\n",myframe->poll);
        PrintOut(PrintHandler, "NTP precision: 0x%X\r\n",myframe->precision);
        PrintOut(PrintHandler, "NTP root delay: %u\r\n",myframe->rootdelay);
        PrintOut(PrintHandler, "NTP drift rate: %u\r\n",myframe->driftrate);
        PrintOut(PrintHandler, "NTP reference id: 0x%X\r\n",myframe->referenceid);
        PrintOut(PrintHandler, "NTP reference time: %lu\r\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP reference fraction: %u\r\n",myframe->reference->timefraction);
        PrintOut(PrintHandler, "NTP origin time: %lu\r\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP origin fraction: %u\r\n",myframe->origin->timefraction);
        PrintOut(PrintHandler, "NTP receive time: %lu\r\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP receive fraction: %u\r\n",myframe->receive->timefraction);
        PrintOut(PrintHandler, "NTP transmit time: %lu\r\n",myframe->reference->timeseconds-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP transmit fraction: %u\r\n",myframe->transmit->timefraction);
        return;
}

void ntp_send(void)
{
        unsigned char IpAddr[4]  = {192, 168,   1,  113 };  // remote IP address
        struct ntpframestruct myframe;
        myframe.leapvermode = 0x1b;
        SPI_Ethernet_arpResolve(IpAddr, 5);
        SPI_Ethernet_sendUDP(IpAddr, 123, 123, (unsigned char *)&myframe, 48);
}