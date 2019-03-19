void PrintHandler(char c) {
     UART1_Write(c);
}

PCF_WrSingle(unsigned char wAddr, unsigned char wData)
{
     unsigned char buf[1];
     buf[0] = wData;
     I2C3_Start();
     I2C3_Write(wAddr,buf,1,END_MODE_STOP);
}

void main() {
     I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_C9);
     UART1_Init(115200);//(stdio/aux3)
     UART1_Write_Text("hello123\r\n");
     UART5_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_EVENPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART5_PC12_PD2);//mdb/exe2
     PCF_WrSingle(0x20, 0b00000000); //mdb/exe2_m/s
     while(1)
     {
      if (UART5_Data_Ready())
        PrintOut(PrintHandler,"RX5 %X\r\n",UART5_Read());
     }
}