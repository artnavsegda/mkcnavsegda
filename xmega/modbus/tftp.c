#include "tftp.h"
#include "global.h"
#include "bswap.h"

unsigned int tftp(unsigned int reqLength)
{
        int opcode = 0;
        static unsigned long filesize;
        unsigned int no_bytes;
        static int block = 0;
        int errcode;
        SPI_Ethernet_getBytes((unsigned char *)&opcode,0xFFFF,2);
        switch(BSWAP_16(opcode))
        {
                case 1:
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-2);
                        if (sd_init != 0)
                        {
                                opcode = BSWAP_16(5);
                                SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                len = 2;
                                errcode = 3;//BSWAP_16(3);
                                SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                len += 2;
                                len += SPI_Ethernet_putString("SD disfunctional");
                                SPI_Ethernet_putByte(0);
                                len++;
                        }
                        else
                        {
                                if (Mmc_Fat_Assign(webpage,0) != 1)
                                {
                                        opcode = BSWAP_16(5);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        errcode = 1;//BSWAP_16(1);
                                        SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                        len += 2;
                                        len += SPI_Ethernet_putString("File not present");
                                        SPI_Ethernet_putByte(0);
                                        len++;
                                }
                                else
                                {
                                        Mmc_Fat_Reset(&filesize);
                                        if (filesize < 512)
                                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                                        else
                                                no_bytes = Mmc_Fat_ReadN(webpage, 512);
                                        opcode = BSWAP_16(3);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        block = BSWAP_16(1);
                                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                                        len += 2;
                                        SPI_Ethernet_putBytes(webpage,no_bytes);
                                        len +=no_bytes;
                                }
                        }
                break;
                case 2:
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-2);
                        if (sd_init != 0)
                        {
                                opcode = BSWAP_16(5);
                                SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                len = 2;
                                len += SPI_Ethernet_putString("SD disfunctional");
                                SPI_Ethernet_putByte(0);
                                len++;
                        }
                        else
                        {
                                if (Mmc_Fat_Assign(webpage,0x80) != 1)
                                {
                                        opcode = BSWAP_16(5);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        errcode = 6;//BSWAP_16(6);
                                        SPI_Ethernet_putBytes((unsigned char *)&errcode,2);
                                        len += 2;
                                        len += SPI_Ethernet_putString("File creation failed");
                                        SPI_Ethernet_putByte(0);
                                        len++;
                                }
                                else
                                {
                                        Mmc_Fat_Rewrite();
                                        opcode = BSWAP_16(4);
                                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                                        len = 2;
                                        block = BSWAP_16(0);
                                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                                        len += 2;
                                }
                        }
                break;
                case 3:
                        SPI_Ethernet_getBytes((unsigned char *)&block,0xFFFF,2);
                        SPI_Ethernet_getBytes(webpage,0xFFFF,reqLength-4);
                        Mmc_Fat_Write(webpage,reqLength-4);
                        opcode = BSWAP_16(4);
                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                        len = 2;
                        //block = BSWAP_16(BSWAP_16(block)+1);
                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                        len += 2;
                        if (reqLength-4 != 512)
                                Mmc_Fat_Close();
                break;
                case 4:
                        SPI_Ethernet_getBytes((unsigned char *)&block,0xFFFF,2);
                        filesize = filesize - 512;
                        if (filesize < 512)
                                no_bytes = Mmc_Fat_ReadN(webpage, filesize);
                        else
                                no_bytes = Mmc_Fat_ReadN(webpage, 512);
                        opcode = BSWAP_16(3);
                        SPI_Ethernet_putBytes((unsigned char *)&opcode,2);
                        len = 2;
                        block = BSWAP_16(BSWAP_16(block)+1);
                        SPI_Ethernet_putBytes((unsigned char *)&block,2);
                        len += 2;
                        SPI_Ethernet_putBytes(webpage,no_bytes);
                        len +=no_bytes;
                break;
        }
        return len;
}
