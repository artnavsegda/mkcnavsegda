#define ADCZERO 0x17CC

extern sfr sbit AD7705_CS;
extern sfr sbit AD7705_DRDY;

void AD7705_Write_Bytes(char *buffer, unsigned NoBytes);
void AD7705_Init(void);
void AD7705_Read_Register(unsigned char addr, unsigned char *value, unsigned int NoBytes);