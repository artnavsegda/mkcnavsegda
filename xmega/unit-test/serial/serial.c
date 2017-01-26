void PrintHandler(char c)
{
        UARTC0_Write(c);
}

char str[100] = "one=1 two=2 three=3.14 ip=192.168.1.150 mac=de:ad:be:ef:fe:ed";

char * getopt(char *config2, char *token)
{
        static char config[100];
        char * pch;
        if (strstr(config2,token)==0)
                return "0";
        strcpy(config,config2);
        pch = strtok(strstr(config,token),"=");
        pch = strtok(0," \n");
        return pch;
}

char * getip(char *config2, char *token)
{
        static char ip[4];
        ip[0] = atoi(strtok(getopt(config2,token),"."));
        ip[1] = atoi(strtok(0,"."));
        ip[2] = atoi(strtok(0,"."));
        ip[3] = atoi(strtok(0,"."));
        return ip;
}

char * getmac(char *config2, char *token)
{
        static char mac[6];
        mac[0] = xtoi(strtok(getopt(config2,token),":"));
        mac[1] = xtoi(strtok(0,":"));
        mac[2] = xtoi(strtok(0,":"));
        mac[3] = xtoi(strtok(0,":"));
        mac[4] = xtoi(strtok(0,":"));
        mac[5] = xtoi(strtok(0,":"));
        return mac;
}

void main() {
        char *ip, *mac;
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        
        PrintOut(PrintHandler, "one int value %d\r\n",atoi(getopt(str,"one")));
        PrintOut(PrintHandler, "two int value %d\r\n",atoi(getopt(str,"two")));
        PrintOut(PrintHandler, "one int value %d\r\n",atoi(getopt(str,"one")));
        PrintOut(PrintHandler, "three three value %f\r\n",atof(getopt(str,"three")));
        ip = getip(str,"ip");
        PrintOut(PrintHandler, "ip %u.%u.%u.%u\r\n",(int)ip[0],(int)ip[1],(int)ip[2],(int)ip[3]);
        mac = getmac(str,"mac");
        PrintOut(PrintHandler, "mac %x:%x:%x:%x:%x:%x\r\n",(int)mac[0],(int)mac[1],(int)mac[2],(int)mac[3],(int)mac[4],(int)mac[5]);

         while (1)
        {
                //PrintOut(PrintHandler, "Hello %x\r\n", 0xDEAD);
                //delay_ms(1000);
        }
}