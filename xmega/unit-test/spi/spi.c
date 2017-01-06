void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPIC_Read(0xFF);
}

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPI_Read(0xFF);
}

void main() {
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
        
         while (1)
        {
                SPIC_Write(0xF0);
        }
        
}