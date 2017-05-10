#include "ntp.h"
#include "timelib.h"
#include "rtc.h"
#include "bswap.h"
#include "log.h"

void ntp_recieve(struct ntpframestruct *myframe)
{
        TimeStruct      ts2;
        long    epoch;
        epoch = BSWAP_32(myframe->reference->timeseconds)-NTP_TIME_OFFSET;
        Time_epochToDate(epoch, &ts2);
        //PrintOut(PrintHandler, "Time is: %d.%d.%d %d:%d:%d\r\n",ts2.yy, (int)ts2.mo, (int)ts2.md, (int)ts2.hh, (int)ts2.mn, (int)ts2.ss);
        Log_Write("NTP recieve, difference is ");
        PrintOut(LogHandler, "%d", rtc_get_time()-epoch);
        rtc_set_time(epoch);
        return;
}

void ntp_send(void)
{
        int result = 3;
        unsigned char IpAddr[4]  = {216, 229,   0,  179 };  // remote IP address
        struct ntpframestruct myframe;
        myframe.leapvermode = 0x1B;
        myframe.poll = 6;
        myframe.stratumlevel = 0;
        myframe.precision = 0xEC;
        SPI_Ethernet_arpResolve(IpAddr, 5);
        result = SPI_Ethernet_sendUDP(IpAddr, 123, 123, (unsigned char *)&myframe, 48);
}