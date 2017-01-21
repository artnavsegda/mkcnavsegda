#define PORTU1 0x18
#define PORTU2 0x19
#define PORTU3 0x1a

#define ADCB_PMT_Voltage 0
#define ADCB_PMT_Current 1
#define ADCB_Flow 2
#define ADCB_Cell 3
#define ADCA_Bypass 6
#define ADCA_Dilution 5
#define ADCA_Vacuum 4

#define ALLOK 0
#define NO_DATA 1
#define LOW_LIGHT 2
#define LOW_FLOW 4
#define CONVERTER 8
#define WATLOW1 16
#define WATLOW2 32
#define WATLOW3 64
#define WATLOW4 128

unsigned char PORTU1_IN, PORTU2_IN, PORTU3_IN;

sfr sbit SERVO_4_RIGHT_IN at PORTU3_IN.B6;
sfr sbit SERVO_2_RIGHT_IN at PORTU2_IN.B7;
sfr sbit SERVO_2_LEFT_IN at PORTU1_IN.B1;
sfr sbit SERVO_3_RIGHT_IN at PORTU2_IN.B3;
sfr sbit SERVO_3_LEFT_IN at PORTU2_IN.B4;

float ADC_Voltage(unsigned int adcvalue)
{
        const float popugai = (3.283/1.603)/4095;
        const int adczero = 186;
        return (adcvalue-adczero)*popugai;
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

int GetStatus(void)
{
        int genstatus = 0;
        if (ADC_Voltage(ADCB_Get_Sample(ADCB_PMT_Voltage)) < 1.0) genstatus |= LOW_LIGHT;
        if (ADC_Voltage(ADCB_Get_Sample(ADCB_Flow)) < 0.0) genstatus |= LOW_FLOW;
        if (SERVO_4_RIGHT_IN) genstatus |= CONVERTER;
        if (SERVO_2_RIGHT_IN) genstatus |= WATLOW1;
        if (SERVO_2_LEFT_IN) genstatus |= WATLOW2;
        if (SERVO_3_RIGHT_IN) genstatus |= WATLOW3;
        if (SERVO_3_LEFT_IN) genstatus |= WATLOW4;
        return genstatus;
}

void Expander_Write_Byte(char ModuleAddress, char RegAddress, char Data_)
{
        TWIE_Start();
        TWIE_Write(ModuleAddress << 1);
        TWIE_Write(RegAddress);
        TWIE_Write(Data_);
        TWIE_Stop();
}

void Expander_Init(char ModuleAddress)
{
        TWIE_Init(100000);
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}

void Ports_Init(void)
{
        Expander_Init(PORTU1);
        Expander_Init(PORTU2);
        Expander_Init(PORTU3);
}

char Expander_Read_Byte(char ModuleAddress, char RegAddress)
{
        unsigned char value;
        TWIE_Start();
        TWIE_Write(ModuleAddress << 1);
        TWIE_Write(RegAddress);
        TWIE_Start();
        TWIE_Write((ModuleAddress<<1)+1);
        value = TWIE_Read(0);
        TWIE_Stop();
        return value;
}

char Expander_Read_Port(char ModuleAddress)
{
        return Expander_Read_Byte(ModuleAddress, 0x00);
}

void main() {
        Ports_Init();
        UARTC0_Init(115200);
        PrintOut(PrintHandler, "START\r\n");
        while (1)
	{
	        delay_ms(1000);
                PORTU1_IN = Expander_Read_Port(PORTU1);
                PORTU2_IN = Expander_Read_Port(PORTU2);
                PORTU3_IN = Expander_Read_Port(PORTU3);
	        PrintOut(PrintHandler, "STATUS: %5d\r\n", GetStatus());
	}
}