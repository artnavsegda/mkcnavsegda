char uart_rd;

void PrintHandler(char c)
{
        UART1_Write(c);
}

void main() {
     char one,two;
  ANSEL  = 0;                     // Configure AN pins as digital
  ANSELH = 0;

  UART1_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(100);                  // Wait for UART module to stabilize

  UART1_Write_Text("Start");
  UART1_Write(10);
  UART1_Write(13);
  
  Delay_ms(100);
  
  SPI1_Init();
  
  Delay_ms(1);
  SPI1_Write(0x20);
  SPI1_Write(0x0C);
  SPI1_Write(0x10);
  SPI1_Write(0x40);
  
  while (1) {                     // Endless loop
  SPI1_Write(0x38);
  one = SPI1_Read(0xFF);
  two = SPI1_Read(0xFF);
  PrintOut(PrintHandler, "%02x%02x\r\n", (int)one,(int)two);
  Delay_ms(100);
  }
}