void main() {
     char string[10];
     int i = 0;
     UART4_Init(9600);
     Delay_ms(100);
     while(1)
     {
             i++;
             sprintf(string,"test %d\r\n",i);
             UART4_Write_Text(string);
             Delay_ms(100);
     }
}