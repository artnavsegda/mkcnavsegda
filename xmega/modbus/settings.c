#include "rtc.h"

char settings [] = "\
ip=192.168.1.160\n\
mac=00:15:a6:72:13:4f\n\
lowlight=2000\n\
lowflow=1000\n\
sll=10\n\
cdl=10\n\
cel=10\n\
zdl=10\n\
ztl=10\n\
prg=10\n\
tdl=10\n\
tml=1000\n\
edl=10\n\
eml=10\n\
rcl=10\n\
cal=10\n\
pcl=10\n\
";
char *options[100];
char *values[100];
int optisize;

void makeopt(void)
{
        int i = 0;
        options[i] = strtok(settings," \r\n");
        while (options[i]!=0)
                options[++i] = strtok(0," \r\n");
        optisize = i;
        for (i=0;i<optisize;i++)
        {
                strtok(options[i],"=");
                values[i] = strtok(0,"=");
        }
}

void breakopt(void)
{
        char datastring[100];
        int i = 0;
        if (Mmc_Fat_Assign("SETTINGS.TXT",0x80) == 1)
        {
                Mmc_Fat_Delete();
                Mmc_Fat_Assign("SETTINGS.TXT",0x80);
                Mmc_Fat_Rewrite();
                for (i=0;i<optisize;i++)
                {
                        sprintf(datastring,"%s=%s\n",options[i],values[i]);
                        Mmc_Fat_Write(datastring,strlen(datastring));
                }
                Mmc_Fat_Close();
        }
}

char * getmyopt(char *parameter)
{
        int i = 0;
        for (i=0;i<optisize;i++)
                if (strcmp(options[i],parameter)==0)
                        return values[i];
        return 0;
}

char * getmyip(char *token)
{
        static char ip[4];
        char *temp = Malloc(strlen(token));
        strcpy(temp,token);
        ip[0] = atoi(strtok(temp,"."));
        ip[1] = atoi(strtok(0,"."));
        ip[2] = atoi(strtok(0,"."));
        ip[3] = atoi(strtok(0,"."));
        Free(temp,sizeof(temp));
        return ip;
}

char * getmymac(char *token)
{
        static char mac[6];
        char *temp = Malloc(strlen(token));
        strcpy(temp,token);
        mac[0] = xtoi(strtok(temp,":"));
        mac[1] = xtoi(strtok(0,":"));
        mac[2] = xtoi(strtok(0,":"));
        mac[3] = xtoi(strtok(0,":"));
        mac[4] = xtoi(strtok(0,":"));
        mac[5] = xtoi(strtok(0,":"));
        Free(temp,sizeof(temp));
        return mac;
}

void setmyopt(char *parameter, char *newset)
{
        int i = 0;
        int found = 0;
        for (i=0;i<optisize;i++)
        {
                if (strcmp(options[i],parameter)==0)
                {
                        found = 1;
                        if (strlen(newset)>strlen(values[i]))
                        {
                                //values[i] = newset; //just change the pointer
                                values[i] = Malloc(strlen(newset)); //or to allocate new memory ?
                                strcpy(values[i],newset);
                        }
                        else
                                strcpy(values[i],newset);
                }
        }
        if (found == 0)
        {
                if (strcmp(parameter,"unixtime")==0)
                {
                        rtc_set_time(atol(newset));
                }
                else
                {
                        options[optisize] = parameter;
                        values[optisize] = newset;
                        optisize++;
                }
        }
}