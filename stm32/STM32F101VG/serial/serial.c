void main() {
     UART4_Init(9600);
     Delay_ms(100);
     while(1)
     {
             UART4_Write_Text("test\r\n");
             Delay_ms(1000);
     }
}