void main() {
        OSCCON = 0x70; // SCS FOSC; OSTS intosc; IRCF 16MHz_HFINTOSC; IDLEN disabled;
        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        while(1)
        {

        }
}