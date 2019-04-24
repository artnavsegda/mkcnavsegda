void PrintHandler(char c) {
     UART6_Write(c);
}

void Uart3_interrupt() iv IVT_INT_USART3 ics ICS_AUTO {
     unsigned rxdata;
     if (UART3_Data_Ready())
     {
      rxdata = UART3_Read();
      UART1_Write(rxdata);
      PrintOut(PrintHandler,"RX3 %c\r\n",rxdata);
     }
}

void Uart1_interrupt() iv IVT_INT_USART1 ics ICS_AUTO {
     unsigned rxdata;
     if (UART1_Data_Ready())
     {
      rxdata = UART1_Read();
      UART3_Write(rxdata);
      PrintOut(PrintHandler,"RX1 %c\r\n",rxdata);
     }
}

void Push_Pwrkey(void)
{
     ODR6_GPIOE_ODR_bit = 0; // PWRKEY down
     Delay_ms(1000);
     ODR6_GPIOE_ODR_bit = 1; // PWRKEY up
}

void SIM800_Start()
{
     if (!IDR15_GPIOG_IDR_bit)
     {
        Push_Pwrkey();
        do { } while (!IDR15_GPIOG_IDR_bit);
     }
}

void SIM800_Stop()
{
     if (IDR15_GPIOG_IDR_bit)
     {
        Push_Pwrkey();
        do { } while (IDR15_GPIOG_IDR_bit);
     }
}

void main() {
     UART1_Init(9600);//(stdio/aux3)
     USART1_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART1); // enable interrupt vector

     UART3_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART3_PC10_PB11); // gsm, also should not work
     USART3_CR1bits.RXNEIE = 1; // enable uart rx interrupt
     NVIC_IntEnable(IVT_INT_USART3); // enable interrupt vector
     
     UART6_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART6_PC67);//(aux1)

     GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_5); // Set PORTE pin 5 as digital output, GSM_PW_ON
     GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_6); // Set PORTE pin 5 as digital output, GSM_PW_KEY
     
     GPIO_Digital_Input(&GPIOG_BASE, _GPIO_PINMASK_15); // Set PORTE pin 5 as digital output, GSM_STATUS

     ODR5_GPIOE_ODR_bit = 1; // enable GSM forever
     Delay_ms(100);
     
     SIM800_Start();

     UART1_Write_Text("start\r\n");
     Delay_ms(5000);
     UART3_Write_Text("AT\n\r");
     Delay_ms(5000);
     UART3_Write_Text("AT\n\r");
     Delay_ms(5000);
     UART1_Write_Text("stop\r\n");
     while(1)
     {
      Delay_ms(1000);
     }
}