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

void main() {
        unsigned int result;
        SPI1_Init();
        SPI_Set_Active(&SPI1_Read, &SPI1_Write);
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
		SPI_Write(0x38);
		SPI_Read_Bytes((unsigned char *)&result,2);
        }
}
