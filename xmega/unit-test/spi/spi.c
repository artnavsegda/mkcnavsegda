void main() {
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read, &SPIC_Write);
}