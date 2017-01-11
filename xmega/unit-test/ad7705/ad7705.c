#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit AD7705_CS at PORTC_OUT.B4;
sfr sbit AD7705_DRDY at PORTC_IN.B1;
sfr sbit LED0_Direction at PORTR_DIR.B0;
sfr sbit LED0_Toggle at PORTR_OUTTGL.B0;

#define MASSIVE_SIZE 64

struct massive {
        unsigned int massive[MASSIVE_SIZE];
        unsigned int position;
};

long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive) // ??????????
{
        int i;
        long x = 0;
        for(i=0; i<amount; i++)
        {
                if (startpos-i>0)
                x=x+selekta[startpos-i-1];
                else
                x=x+selekta[sizeofmassive+(startpos-i)-1];
        }
        return x;
}

long oversample(struct massive *working, unsigned int amount)
{
        return average(working->massive,amount,working->position,sizeof(working->massive)/2);
}

void increment(struct massive *working, unsigned int value)
{
        working->massive[working->position] = value;
                if (working->position++ > sizeof(working->massive)/2)
        working->position = 0;
}

struct massive firststage;

void AD7705_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        AD7705_CS = 0;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
        AD7705_CS = 1;
}

void AD7705_Init(void)
{
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        AD7705_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
}

void AD7705_Read_Register(unsigned char addr, unsigned char *value, unsigned int NoBytes)
{
        int i;
        AD7705_CS = 0;
        SPIC_Write(addr);
        for (i = 0; i < NoBytes; i++)
                value[i] = SPIC_Read(0xFF);
        AD7705_CS = 1;
}

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
        unsigned int result;
        LED0_Direction = 1;
        UARTC0_Init(115200);
        AD7705_Init();
        delay_ms(10);
        AD7705_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        AD7705_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);
        
        while (1)
        {
                if (AD7705_DRDY == 0)
                {
                        LED0_Toggle = 1;
                        AD7705_Read_Register(0x38,(unsigned char *)&result,2);
                        increment(&firststage,BSWAP_16(result));
                        
                        PrintOut(PrintHandler, "raw: %5d, ", BSWAP_16(result)-0x17CC);
                        PrintOut(PrintHandler, "x16: %5d\r\n", (oversample(&firststage,64)/64)-0x17CC);
                  }
        }
}