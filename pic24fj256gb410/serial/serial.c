void main() {
    TRISA = 0xC6FF;
    TRISB = 0xFFFF;
    TRISC = 0x901E;
    TRISD = 0xFFFF;
    TRISE = 0x03FF;
    TRISF = 0x31B7;
    TRISG = 0xF3CF;
    
    ANSA = 0xC6ED;
    ANSB = 0xFFFC;
    ANSC = 0x001E;
    ANSD = 0xFFFF;
    ANSE = 0x03FF;
    ANSF = 0x313B;
    ANSG = 0xF3C3;
    ANSH = 0x001F;
    
     Unlock_IOLOCK();
     //PPS_Mapping(30, _INPUT, _U1RX);
     //PPS_Mapping(16, _OUTPUT, _U1TX);
     
    RPOR8bits.RP16R = 0x0003;   //RF3->UART1:U1TX;
    RPINR18bits.U1RXR = 0x001E;   //RF2->UART1:U1RX;
     
     
     Lock_IOLOCK();

     UART1_Init(9600);
     UART1_Write_Text("Hello world\r\n");
     
     while(1);
    
}