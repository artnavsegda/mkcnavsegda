#include "prototype.h"
#include "modbus.h"

unsigned int table[100] = {0x0000, 0x0000, 0x0000};
unsigned char bctable[200] = {0x0, 0x0, 0x0, 0x0, 0x0};

void splitfloat(unsigned int *wordlow, unsigned int *wordhigh, float value)
{
        wordlow = ((int *)&value)[0];
        wordhigh = ((int *)&value)[1];
}

unsigned int modbus(struct mbframestruct *askframe)
{
        int amount = 100;
        int bcamount = 200;
        int i;
        int firstrequest = 0;
        int requestnumber = 0;
        switch (askframe->pdu->fncode)
        {
                case 1:
                case 2:
                        firstrequest = BSWAP_16(askframe->pdu->values->askreadregs->firstreg);
                        requestnumber = BSWAP_16(askframe->pdu->values->askreadregs->regnumber);
                        askframe->pdu.values->reqreadcoils->bytestofollow = requestnumber / 8;
                        if ((requestnumber % 8)>0)
                           askframe->pdu->values->reqreadcoils->bytestofollow++;
                        askframe->length = BSWAP_16(askframe->pdu->values->reqreadcoils->bytestofollow + 3);
                        for (i = 0; i < requestnumber/8; i++)
                                if(firstrequest+i < bcamount)
                                        if (bctable[firstrequest+i] != 0)
                                                askframe->pdu->values->reqreadcoils->coils[i/8] = askframe->pdu->values->reqreadcoils->coils[i/8] & (0x01 << i%8);
                                        else
                                                askframe->pdu->values->reqreadcoils->coils[i/8] = askframe->pdu->values->reqreadcoils->coils[i/8] | ~(0x01 << i%8);
                                else
                                        askframe->pdu->values->reqreadcoils->coils[i/8] = askframe->pdu->values->reqreadcoils->coils[i/8] | ~(0x01 << i%8);
                break;
                case 3:
                case 4:
                        firstrequest = BSWAP_16(askframe->pdu->values->askreadregs->firstreg);
                        requestnumber = BSWAP_16(askframe->pdu->values->askreadregs->regnumber);
                        askframe->pdu->values->reqreadholdings->bytestofollow = requestnumber * 2;
                        askframe->length = BSWAP_16(askframe->pdu->values->reqreadholdings->bytestofollow + 3);
                        for (i = 0; i < requestnumber;i++)
                            if(firstrequest+i < amount) // if requested register within allocated range
                                askframe->pdu->values->reqreadholdings->registers[i] = BSWAP_16(table[firstrequest+i]);
                            else
                                askframe->pdu->values->reqreadholdings->registers[i] = BSWAP_16(0x0000); // fill up with zeroes
                break;
                case 5:
                        if (BSWAP_16(askframe->pdu->values->writereg->regaddress) < bcamount)
                                if (askframe->pdu->values->writereg->regvalue == 0)
                                        bctable[BSWAP_16(askframe->pdu->values->writereg->regaddress)] = 0;
                                else
                                        bctable[BSWAP_16(askframe->pdu->values->writereg->regaddress)] = 1;
                break;
                case 6:
                        if (BSWAP_16(askframe->pdu->values->writereg->regaddress) < amount)
                                table[BSWAP_16(askframe->pdu->values->writereg->regaddress)] = BSWAP_16(askframe->pdu->values->writereg->regvalue);
                break;
                case 15:
                        for (i = 0; i < BSWAP_16(askframe->pdu->values->writemulticoil->regnumber);i++)
                                if (BSWAP_16(askframe->pdu->values->writemulticoil->firstreg)+i < bcamount)
                                        if (askframe->pdu->values->writemulticoil->coils == 0)
                                                bctable[BSWAP_16(askframe->pdu->values->writemulticoil->firstreg)+i] = 0;
                                        else
                                                bctable[BSWAP_16(askframe->pdu->values->writemulticoil->firstreg)+i] = 1;
                break;
                case 16:
                        for (i = 0; i < BSWAP_16(askframe->pdu->values->writemultireg->regnumber);i++)
                                if (BSWAP_16(askframe->pdu->values->writemultireg->firstreg)+i < amount)
                                        table[BSWAP_16(askframe->pdu->values->writemultireg->firstreg)+i] = BSWAP_16(askframe->pdu->values->writemultireg->registers[i]);
                        askframe->length = BSWAP_16(6);
                default:
                break;
        }

        return BSWAP_16(askframe->length) + 6;
}