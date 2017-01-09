#include  <__EthEnc28j60.h>

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;
sfr sbit ad7705_Chip_Select at PORTC_OUT.B4;
sfr sbit ad7705_Chip_Select_Direction at PORTC_DIR.B4;
sfr sbit ad7707_drdy at PORTC_IN.B1;

struct askreadregstruct {
        unsigned int firstreg;
        unsigned int regnumber;
};

struct reqreadcoilsstruct {
        unsigned char bytestofollow;
        unsigned char coils[256];
};

struct reqreadwordstruct {
        unsigned char bytestofollow;
        unsigned int registers[127];
};

struct writeregstruct {
        unsigned int regaddress;
        unsigned int regvalue;
};

struct writemulticoilstruct {
        unsigned int firstreg;
        unsigned int regnumber;
        unsigned char bytestofollow;
        unsigned char coils[256];
};

struct writemultiregstruct {
        unsigned int firstreg;
        unsigned int regnumber;
        unsigned char bytestofollow;
        unsigned int registers[127];
};

union pdudataunion {
        struct askreadregstruct askreadregs;
        struct reqreadcoilsstruct reqreadcoils;
        struct reqreadwordstruct reqreadholdings;
        struct writeregstruct writereg;
        struct writemulticoilstruct writemulticoil;
        struct writemultiregstruct writemultireg;
        unsigned int words[128];
        unsigned char bytes[256];
};

struct pduframestruct {
        unsigned char unitid;
        unsigned char fncode;
        union pdudataunion values;
};

struct mbframestruct {
        unsigned int tsid;
        unsigned int protoid;
        unsigned int length;
        struct pduframestruct pdu;
};

struct mbframestruct askframe;

unsigned int table[100] = {0xABCD, 0xDEAD, 0x0000};
unsigned char bctable[100] = {0x0, 0x1, 0x0, 0x0, 0x1};
unsigned char crmassive[100];
unsigned int amount = 100;
unsigned int result;

void PrintHandler(char c)
{
        UART_Write(c);
}

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        int i;
        unsigned int len;            // my reply length
        int firstrequest = 0;
        int requestnumber = 0;

        // should we close tcp socket after response is sent?
        // library closes tcp socket by default if canClose flag is not reset here
        flags->canCloseTCP = 0; // 0 - do not close socket
        // otherwise - close socket

        if(localPort != 502)             // I listen only to web request on port 80
        {
                return(0) ;
        }

        PrintOut(PrintHandler, "Recieved %d bytes\r\n", reqLength);

        /*for(i = 0;i < reqLength; i++)
        {
        //        buf[i] = SPI_Ethernet_getByte();
                ((char *)&askframe)[i] = SPI_Ethernet_getByte();
        //        Spi_Ethernet_putByte(buf[i]);//send back all recieved modbus packets
        //        PrintOut(PrintHandler, "%c ", buf[i]); // print ascii chars
        //        PrintOut(PrintHandler, "%#02x ", (unsigned int)buf[i]); // print hex bytes
                PrintOut(PrintHandler, "%#02x ", (unsigned int)((char *)&askframe)[i]); // print hex bytes
        //        PrintOut(PrintHandler, "%u ", (unsigned int)buf[i]); // print unsigned numbers
        }
        UART_Write_Text("\r\n");*/
        SPI_Ethernet_getBytes((unsigned char *)&askframe, 0xFFFF, reqLength);
        
        PrintOut(PrintHandler, "TS id: %u\r\n", BSWAP_16(askframe.tsid));
        PrintOut(PrintHandler, "Protocol id: %u\r\n", BSWAP_16(askframe.protoid));
        PrintOut(PrintHandler, "Length PDU unit: %u\r\n", BSWAP_16(askframe.length));
        
        PrintOut(PrintHandler, "Unit id: %u\r\n", (unsigned int)askframe.pdu.unitid);
        PrintOut(PrintHandler, "Function code: %u\r\n", (unsigned int)askframe.pdu.fncode);

        switch (askframe.pdu.fncode)
        {
                case 1:
                case 2:
                        firstrequest = BSWAP_16(askframe.pdu.values.askreadregs.firstreg);
                        PrintOut(PrintHandler, "Requesing coils starting from:: %d\r\n", firstrequest);
                        requestnumber = BSWAP_16(askframe.pdu.values.askreadregs.regnumber);
                        PrintOut(PrintHandler, "Number of C/D registers requested: %d\r\n", requestnumber);
                        askframe.pdu.values.reqreadcoils.bytestofollow = requestnumber / 8;
                        if ((requestnumber % 8)>0)
                           askframe.pdu.values.reqreadcoils.bytestofollow++;
                        askframe.length = BSWAP_16(askframe.pdu.values.reqreadcoils.bytestofollow + 3);
                        // fill all requested coil bytes with zeroes
                        for (i = 0; i < requestnumber/8; i++)
                                if(firstrequest+i < amount)
                                        if (bctable[firstrequest+i] != 0)
                                                askframe.pdu.values.reqreadcoils.coils[i/8] = askframe.pdu.values.reqreadcoils.coils[i/8] & (0x01 << i%8);
                                        else
                                                askframe.pdu.values.reqreadcoils.coils[i/8] = askframe.pdu.values.reqreadcoils.coils[i/8] | ~(0x01 << i%8);
                                else
                                        askframe.pdu.values.reqreadcoils.coils[i/8] = askframe.pdu.values.reqreadcoils.coils[i/8] | ~(0x01 << i%8);
                break;
                case 3:
                case 4:
                        firstrequest = BSWAP_16(askframe.pdu.values.askreadregs.firstreg);
                        PrintOut(PrintHandler, "Requesing register starting from:: %d\r\n", firstrequest);
                        requestnumber = BSWAP_16(askframe.pdu.values.askreadregs.regnumber);
                        PrintOut(PrintHandler, "Numer of H/I registers requested: %d\r\n", requestnumber);
                        askframe.pdu.values.reqreadholdings.bytestofollow = requestnumber * 2;
                        askframe.length = BSWAP_16(askframe.pdu.values.reqreadholdings.bytestofollow + 3);
                        // fill every requested register with 0xABCD
                        for (i = 0; i < requestnumber;i++)
                            if(firstrequest+i < amount) // if requested register within allocated range
                                askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(table[firstrequest+i]);
                            else
                                askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(0x0000); // fill up with zeroes
                break;
                case 5:
                        if (BSWAP_16(askframe.pdu.values.writereg.regaddress) < amount)
                                if (askframe.pdu.values.writereg.regvalue == 0)
                                        bctable[BSWAP_16(askframe.pdu.values.writereg.regaddress)] = 0;
                                else
                                        bctable[BSWAP_16(askframe.pdu.values.writereg.regaddress)] = 1;
                break;
                case 6:
                        if (BSWAP_16(askframe.pdu.values.writereg.regaddress) < amount)
                                table[BSWAP_16(askframe.pdu.values.writereg.regaddress)] = BSWAP_16(askframe.pdu.values.writereg.regvalue);
                break;
                case 15:
                        for (i = 0; i < BSWAP_16(askframe.pdu.values.writemulticoil.regnumber);i++)
                                if (BSWAP_16(askframe.pdu.values.writemulticoil.firstreg)+i < amount)
                                        if (askframe.pdu.values.writemulticoil.coils == 0)
                                                bctable[BSWAP_16(askframe.pdu.values.writemulticoil.firstreg)+i] = 0;
                                        else
                                                bctable[BSWAP_16(askframe.pdu.values.writemulticoil.firstreg)+i] = 1;
                break;
                case 16:
                        for (i = 0; i < BSWAP_16(askframe.pdu.values.writemultireg.regnumber);i++)
                                if (BSWAP_16(askframe.pdu.values.writemultireg.firstreg)+i < amount)
                                        table[BSWAP_16(askframe.pdu.values.writemultireg.firstreg)+i] = BSWAP_16(askframe.pdu.values.writemultireg.registers[i]);
                        askframe.length = BSWAP_16(6);
                default:
                break;
        }
        
        len = BSWAP_16(askframe.length) + 6;
        PrintOut(PrintHandler, "Reply length: %d\r\n", len);
        
        SPI_Ethernet_putBytes((unsigned char *)&askframe,len);
 
        //return(reqLength);//send back all recieved bytes
        return(len);
        //return(0);
}

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return(0);
}

int tick;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF {
        tick = 1;
}

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                buffer[i] = SPIC_Read(0xFF);
}

void SPIC_Write_Bytes(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
                SPIC_Write(buffer[i]);
}

void ad7705_init(void)
{
        SPI_Ethernet_CS = 1;
        ad7705_Chip_Select = 0;

        SPIC_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
        delay_ms(10);
        SPIC_Write_Bytes("\x20\x0C\x10\x04", 4);
        delay_ms(10);
        SPIC_Write_Bytes("\x60\x18\x3A\x00", 4);
        delay_ms(10);
        SPIC_Write_Bytes("\x70\x89\x78\xD7", 4);
        
        ad7705_Chip_Select = 1;
        SPI_Ethernet_CS = 0;
}

/*
 * main entry
 */
void    main()
{
        int i;
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        UART_Write_Text("Starting\r\n");
        PrintOut(PrintHandler, "Testing output\r\n");
        PMIC_CTRL = 4;                    // Enable medium level interrupts
        CPU_SREG.B7 = 1;                  // Enable global interrupts
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
        //SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
        SPIC_Init();
        SPI_Set_Active(&SPIC_Read,&SPIC_Write);
        ad7705_init();
        Spi_Rd_Ptr = SPIC_Read;
        SPI_Ethernet_Init("\x00\x14\xA5\x76\x19\x3f", "\xC0\xA8\x01\x96", 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");

        while(1) // do forever
        {
                /*
                 * if necessary, test the return value to get error code
                 */
                SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
                
                if (ad7707_drdy == 0)
                {
                        //PORTD_OUTTGL.B5 = 1;
                        ad7705_Chip_Select = 0;
                        SPIC_Write(0x38);
                        SPIC_Read_Bytes((unsigned char *)&result,2);
                        ad7705_Chip_Select = 1;
                }

                if (tick == 1)
                {
                        PORTD_OUTTGL.B4 = 1;
                        table[2]++;
                        table[3] = BSWAP_16(result);
                        for (i = 0; i<8;i++)
                                table[4+i] = ADCA_Read(i);
                        for (i = 0; i<8;i++)
                                table[12+i] = ADCB_Read(i);
                        tick = 0;
                        //PrintOut(PrintHandler, "cs0 %x\r\n", BSWAP_16(result));
                }
        }
}