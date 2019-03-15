void main() {
     char exetx, exerx;
     char string[10];
     int i = 0;
     UART1_Init(9600);//(RX+/-)
     UART3_Init(9600);//(TX+/-)
     UART4_Init(115200);//stdio
     Delay_ms(100);
     while(1)
     {
             if (UART1_Data_Ready())
             {
                exerx = UART1_Read();
                sprintf(string,"RX %d\r\n",exerx);
                UART4_Write_Text(string);
             }
             if (UART3_Data_Ready())
             {
                exetx = UART3_Read();
                sprintf(string,"TX %d\r\n",exetx);
                UART4_Write_Text(string);
             }
             //i++;
             //sprintf(string,"test %d\r\n",i);
             //UART4_Write_Text(string);
             //Delay_ms(100);
     }
}