void main() {
     UART1_Init(9600);
     Delay_ms(100);
     while(1)
     {
             UART1_Write_Text("test\r\n");
             Delay_ms(1000);
     }
}