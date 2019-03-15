long long num = 0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
     TIM2_SR.UIF = 0;
     num++;
}

void main() {
     char exetx, exerx;
     char string[10];
     int i = 0;
     
     RCC_APB1ENR.TIM2EN = 1;       // Enable clock gating for timer module 2
     TIM2_CR1.CEN = 0;             // Disable timer
     TIM2_PSC = 0;              // Set timer prescaler.
     TIM2_ARR = 7999;
     NVIC_IntEnable(IVT_INT_TIM2); // Enable timer interrupt
     TIM2_DIER.UIE = 1;            // Update interrupt enable
     TIM2_CR1.CEN = 1;             // Enable timer
     
     
     
     UART1_Init(9600);//(RX+/-)
     UART3_Init(9600);//(TX+/-)
     UART4_Init(115200);//stdio
     Delay_ms(100);
     while(1)
     {
             if (UART1_Data_Ready())
             {
                exerx = UART1_Read();
                sprintf(string,"RX1 %X %lu\r\n",exerx,num);
                UART4_Write_Text(string);
             }
             if (UART3_Data_Ready())
             {
                exetx = UART3_Read();
                sprintf(string,"RX3 %X %lu\r\n",exetx,num);
                UART4_Write_Text(string);
             }
     }
}
