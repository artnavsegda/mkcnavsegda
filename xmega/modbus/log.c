#include "global.h"
#include "timelib.h"
#include "rtc.h"
#include <__Lib_MmcFat16.h>

short logfile;

void LogHandler(char c)
{
        if (sd_init == 0)
        {
                Mmc_Fat_Write(&c, 1);
                //UART_Write(c);
        }
}

void Log_Write(char message[])
{
        static TimeStruct ts2;
        if (sd_init == 0)
        {
                Mmc_Fat_Activate(logfile);
                Time_epochToDate(rtc_get_time(), &ts2);
                PrintOut(LogHandler, "\r\n%4d.%02d.%02d %02d:%02d:%02d: ", ts2.yy,(int)ts2.mo,(int)ts2.md,(int)ts2.hh,(int)ts2.mn,(int)ts2.ss);
                Mmc_Fat_Write(message, strlen(message));
                //UART_Write_Text(message);
        }
}

void Log_Init(void)
{
        if (sd_init == 0)
        {
                logfile = Mmc_Fat_Open("LOG.TXT",FILE_APPEND,0x80);
                Mmc_Fat_Append();
                Log_Write("Device started");
        }
}