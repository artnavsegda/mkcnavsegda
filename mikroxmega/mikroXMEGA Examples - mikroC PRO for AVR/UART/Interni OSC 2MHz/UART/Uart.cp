#line 1 "E:/Slavisa/ceves/Support/Testiranje - evidencija/Hardware/2010-09-02/XMEGA-Ready HW REV 1.01/Prilozi/Primeri_za_testiranje/XMEGA/mC/UART/Uart.c"
#line 24 "E:/Slavisa/ceves/Support/Testiranje - evidencija/Hardware/2010-09-02/XMEGA-Ready HW REV 1.01/Prilozi/Primeri_za_testiranje/XMEGA/mC/UART/Uart.c"
char uart_rd;
void main() {

 UARTC0_Init(19200);
 Delay_ms(1000);

 UARTC0_Write_Text("Start");

 while (1) {
 if (UARTC0_Data_Ready()) {
 uart_rd = UARTC0_Read();
 UARTC0_Write(uart_rd);
 }
 }

}
