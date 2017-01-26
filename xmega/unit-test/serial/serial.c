void PrintHandler(char c)
{
        UARTC0_Write(c);
}

char str[100] = "one=1 two=2 three=3.14 ip=192.168.1.150";

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

void main() {
        char *ip;
        UARTC0_Init(9600);
	UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
	
	PrintOut(PrintHandler, "one int value %d\r\n",atoi(getopt(str,"one")));
	PrintOut(PrintHandler, "two int value %d\r\n",atoi(getopt(str,"two")));
	PrintOut(PrintHandler, "one int value %d\r\n",atoi(getopt(str,"one")));
	PrintOut(PrintHandler, "three three value %f\r\n",atof(getopt(str,"three")));
	ip = getip(str,"ip");
	PrintOut(PrintHandler, "ip %u.%u.%u.%u\r\n",(int)ip[0],(int)ip[1],(int)ip[2],(int)ip[3]);

         while (1)
        {
                //PrintOut(PrintHandler, "Hello %x\r\n", 0xDEAD);
                //delay_ms(1000);
        }
}