char settings [100] = "\
ip=192.168.1.150\n\
mac=00:14:a5:76:19:3f\n\
";

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
