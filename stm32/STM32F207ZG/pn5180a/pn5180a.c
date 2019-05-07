void PrintHandler(char c) {
     UART1_Write(c);
}

void main() {
     UART1_Init(9600);//(stdio/aux3)
}