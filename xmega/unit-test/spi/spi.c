#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
}

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPIC_Read(0xFF);
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
}

void SPIC_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPIC_Write(buffer[i]);
}

void main() {
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(100);
        SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(100);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(100);

	while (1)
        {
                SPIC_Write(0x38);
                SPIC_Read_Bytes((unsigned char *)&result,2);
        }
        
}

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sbit Chip_Select at PORTC_OUT.B4;
//sbit Chip_Select_Direction at PORTC_DIR.B4;
sbit ad7707_drdy at PORTC_IN.B1;
//sbit ad7707_drdy_direction at PORTC_DIR.B1;

sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;

void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void nemain() {
        int x, y;
        unsigned int result;
        //Chip_Select_Direction = 0; // ad7705 cs
        //PORTC_DIR.B1 = 1;
        PORTD_DIR.B5 = 1;
        delay_ms(100);

        UARTC0_Init(115200);

        delay_ms(100);

        UARTC0_Write_Text("hello\r\n");

        delay_ms(100);

        SPIC_Init();

        delay_ms(100);

        SPI_Ethernet_CS = 1; // block ethernet

        Chip_Select = 0;

        SPIC_Write(0xFF);
        SPIC_Write(0xFF);
        SPIC_Write(0xFF);
        SPIC_Write(0xFF);
        SPIC_Write(0xFF);

        delay_ms(100);

        SPIC_Write(0x20);
        SPIC_Write(0x0C);
        SPIC_Write(0x10);
        SPIC_Write(0x04);

        delay_ms(100);

        SPIC_Write(0x60);
        SPIC_Write(0x18);
        SPIC_Write(0x3A);
        SPIC_Write(0x00);

        delay_ms(100);

        SPIC_Write(0x70);
        SPIC_Write(0x89);
        SPIC_Write(0x78);
        SPIC_Write(0xD7);
        Chip_Select = 1;

        while(1)
        {
                //delay_ms(100);

                //PrintOut(PrintHandler, "drdy status is %d\r\n",PORTC_IN.B1);

                if (ad7707_drdy == 0)
                //        UARTC0_Write_Text("drdy 0\r\n");
                //else
                //        UARTC0_Write_Text("drdy 1\r\n");

                {
                  PORTD_OUTTGL.B5 = 1;
                  Chip_Select = 0;
                  SPIC_Write(0x38);
                  //x = SPIC_Read(0xFF);
                  //y = SPIC_Read(0xFF);
                  //((unsigned char)&result)[0] = SPIC_Read(0xFF);
                  //((unsigned char)&result)[1] = SPIC_Read(0xFF);
                  SPIC_Read_Bytes((unsigned char *)&result,2);
                  Chip_Select = 1;
                }

                //delay_ms(100);

                //PrintOut(PrintHandler, "cs0 %x %x\r\n",(unsigned int)x,(unsigned int)y);
                PrintOut(PrintHandler, "cs0 %x\r\n", BSWAP_16(result));

                //UARTC0_Write(x);
                //UARTC0_Write(y);
        }
}