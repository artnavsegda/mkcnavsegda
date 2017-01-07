void TWIE_Send(unsigned char devaddr, unsigned char regaddr, unsigned char value)
{
        TWIE_Start();
        TWIE_Write(devaddr << 1);
        TWIE_Write(regaddr);
        TWIE_Write(value);
        TWIE_Stop();
}

unsigned char TWIE_Recieve(unsigned char devaddr, unsigned char regaddr)
{
	unsigned char value;
        TWIE_Start();
        TWIE_Write(devaddr << 1);
        TWIE_Write(regaddr);
        TWIE_Start();
        TWIE_Write((devaddr<<1)+1);
        value = TWIE_Read(0);
        TWIE_Stop();
        return value;
}

void main()
{
        TWIE_Init(100000);
        TWIE_Send(0x1a,0x03,0x00);
        TWIE_Send(0x1a,0x02,0x00);

        while (1)
        {
                Delay_100ms();
                TWIE_Send(0x1a,0x01,0x00);
                Delay_100ms();
                TWIE_Send(0x1a,0x01,0xFF);
        }
}