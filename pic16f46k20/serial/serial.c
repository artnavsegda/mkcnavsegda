void main() {
        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
}