void main() {
     char string[10];
     UART1_Init(115200);//(stdio/aux3)
     UART5_Init(9600);//mdb/exe2
     UART1_Write_Text("hello123\r\n");
     while(1)
     {
      if (UART5_Data_Ready())
      {
        sprintf(string,"RX5 %X\r\n",UART5_Read());
        UART1_Write_Text(string);
      }
     }
}