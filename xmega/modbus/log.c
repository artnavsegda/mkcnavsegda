#include "global.h"
#include "timelib.h"
#include "rtc.h"
#include <__Lib_MmcFat16.h>

short logfile;

void LogHandler(char c)
{
        Mmc_Fat_Write(&c, 1);
        //UART_Write(c);
}

void Log_Write(char message[])
{
	static TimeStruct ts2;
        Mmc_Fat_Activate(logfile);
        Time_epochToDate(rtc_get_time(), &ts2);
        PrintOut(LogHandler, "%d.%d.%d %d:%d:%d: ", ts2.yy,ts2.mo,ts2.md,ts2.hh,ts2.mn,ts2.ss);
        Mmc_Fat_Write(message, strlen(message));
        //UART_Write_Text(message);
        Mmc_Fat_Write("\r\n", 2);
        //UART_Write_Text("\r\n");
}

void Log_Init(void)
{
        if (sd_init == 0)
        {
                logfile = Mmc_Fat_Open("LOG.TXT",FILE_APPEND,0x80);
                Log_Write("Device started");
        }
}