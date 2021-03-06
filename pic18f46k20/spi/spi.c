#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPI_Write(buffer[i]);
}

void PrintHandler(char c)
{
        UART1_Write(c);
}

void main() {
        unsigned int result;
        OSCCON = 0x70; // SCS FOSC; OSTS intosc; IRCF 16MHz_HFINTOSC; IDLEN disabled;
        UART1_Init(9600);
        Delay_ms(10);
        UART1_Write_Text("MCU-Started\r\n");
        SPI1_Init();
        SPI_Set_Active(&SPI1_Read, &SPI1_Write);
        SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(10);
        SPI_Write_Bytes("\x20\x0C",2);
        delay_ms(10);
        SPI_Write_Bytes("\x10\x04", 2);
        delay_ms(10);
        SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
        delay_ms(10);
     
        while(1)
        {
                delay_ms(100);
                SPI_Write(0x38);
                SPI_Read_Bytes((unsigned char *)&result,2);
                PrintOut(PrintHandler, "hex: %5x\r\n", BSWAP_16(result));
        }
}
