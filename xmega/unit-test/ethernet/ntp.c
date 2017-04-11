#include "ntp.h"
#include "timelib.h"

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#define BSWAP_32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void ntp_recieve(struct ntpframestruct *myframe)
{
        TimeStruct      ts2;
        long    epoch;
        PrintOut(PrintHandler, "ntp frame size is %d\r\n",(int)sizeof(struct ntpframestruct));
        PrintOut(PrintHandler, "NTP lvm: 0x%X\r\n",(int)myframe->leapvermode);
        PrintOut(PrintHandler, "NTP stratum: 0x%X\r\n",(int)myframe->stratumlevel);
        PrintOut(PrintHandler, "NTP poll: 0x%X\r\n",(int)myframe->poll);
        PrintOut(PrintHandler, "NTP precision: 0x%X\r\n",(int)myframe->precision);
        PrintOut(PrintHandler, "NTP root delay: %u\r\n",myframe->rootdelay);
        PrintOut(PrintHandler, "NTP drift rate: %u\r\n",myframe->driftrate);
        PrintOut(PrintHandler, "NTP reference id: 0x%X\r\n",myframe->referenceid);
        PrintOut(PrintHandler, "NTP reference time: %lu\r\n",BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP reference fraction: %lu\r\n",BSWAP_32(myframe->reference->timefraction));
        PrintOut(PrintHandler, "NTP origin time: %lu\r\n",BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP origin fraction: %lu\r\n",BSWAP_32(myframe->origin->timefraction));
        PrintOut(PrintHandler, "NTP receive time: %lu\r\n",BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP receive fraction: %lu\r\n",BSWAP_32(myframe->receive->timefraction));
        PrintOut(PrintHandler, "NTP transmit time: %lu\r\n",BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET);
        PrintOut(PrintHandler, "NTP transmit fraction: %lu\r\n",BSWAP_32(myframe->transmit->timefraction));
        epoch = BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET;
        Time_epochToDate(epoch, &ts2);
        PrintOut(PrintHandler, "Time is: %d.%d.%d %d:%d:%d\r\n",ts2.yy, (int)ts2.mo, (int)ts2.md, (int)ts2.hh, (int)ts2.mn, (int)ts2.ss);
        return;
}

void ntp_send(void)
{
        int result = 3;
        //unsigned char IpAddr[4]  = {132, 163,   4,  103 };  // remote IP address
        //unsigned char IpAddr[4]  = {192, 168,   1,  113 };  // remote IP address
        unsigned char IpAddr[4]  = {216, 229,   0,  179 };  // remote IP address
        struct ntpframestruct myframe;
        myframe.leapvermode = 0x1B;
        myframe.poll = 6;
        myframe.stratumlevel = 0;
        myframe.precision = 0xEC;
        SPI_Ethernet_arpResolve(IpAddr, 5);
        result = SPI_Ethernet_sendUDP(IpAddr, 123, 123, (unsigned char *)&myframe, 48);
        PrintOut(PrintHandler, "result: %d\r\n",result);
}