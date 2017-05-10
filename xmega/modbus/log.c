#include "global.h"
#include <__Lib_MmcFat16.h>

short logfile;

void Log_Write(char message[])
{
        Mmc_Fat_Activate(logfile);
        Mmc_Fat_Write(message, strlen(message));
}

void Log_Init(void)
{
        if (sd_init == 0)
        {
                logfile = Mmc_Fat_Open("LOG.TXT",FILE_APPEND,0x80);
                Log_Write("Device started");
	}
}