#include  <__EthEnc28j60.h>
#include "modbus.h"

#define TRANSMIT_START  0x19AE
#define REPLY_START     (TRANSMIT_START + 1)            // reply buffer starts after per packet control byte
#define ARPCACHELEN 3

// some ENC28J60 registers, see datasheet
#define ERDPTL  0x00
#define ERDPTH  0x01
#define ECON1   0x1f
#define EDMACSL 0x16
#define EDMACSH 0x17

struct {
        unsigned char   valid;
        unsigned long   tmr;
        unsigned char   ip[4];
        unsigned char   mac[6];
} arpCache[ARPCACHELEN];

unsigned char   tmr = 0;
unsigned long   longTmr = 0;

///// TEMPORARY
unsigned char   macAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f};   // my MAC address
unsigned char   ipAddr[4] = {192, 168, 1, 150};                      // my IP address

unsigned char   ntpserver[4] = {128,138,140,44};                      // ntp server IP address

/*
 * ARP resolution
 * this function returns the MAC address of an IP address
 * an ARP cache is used
 */
unsigned char   *arpResolv(unsigned char *ip)
{
        unsigned char   i ;             // general purpose register
        unsigned int    rdptr ;         // backup of ENC read pointer register
        unsigned char   freeSlot ;      // index of free slot in ARP cache
        unsigned long   tmrSlot ;       // oldest slot timer in ARP cache

        tmrSlot = 0xffffffff ;  // will try to find less than this !

        // step in ARP cache
        for(i = 0 ; i < ARPCACHELEN;  i++)
        {
                // oldest slots must expire
                if(arpCache[i].tmr < longTmr - 1000)        // too old ?
                        {
                        arpCache[i].tmr = 0 ;           // clear timer
                        arpCache[i].valid = 0 ;         // disable slot
                        }

                // is it the address I want in a valid slot ?
                if(arpCache[i].valid && (memcmp(arpCache[i].ip, ip, 4) == 0))
                        {
                        return(arpCache[i].mac) ;       // yes, return ready to use MAC addr
                        }

                // find older slot, will be needed further
                if(arpCache[i].tmr < tmrSlot)
                        {
                        tmrSlot = arpCache[i].tmr ;     // save best timer
                        freeSlot = i ;                  // save best index
                        }
        }

        // select ENC register bank 0
        Spi_Ethernet_clearBitReg(ECON1, 0b00000001) ;
        Spi_Ethernet_clearBitReg(ECON1, 0b00000010) ;

        /***************************
         * first, build eth header
         */
        Spi_Ethernet_memcpy(REPLY_START, "\xff\xff\xff\xff\xff\xff", 6) ;          // destination is broadcast
        Spi_Ethernet_memcpy(REPLY_START + 6, macAddr, 6) ;      // source : my mac addr
        Spi_Ethernet_writeMemory(REPLY_START + 12, 8, 6) ;   // protocol is ARP

        /****************************
         * second, build ARP packet
         */

        /*
         * 0, 1 : hardware type = ethernet
         * 8, 0 : protocole type = ip
         * 6, 4 : HW size = 6, protocol size = 4
         * 0, 1 : ARP opcode = request
         */
        Spi_Ethernet_memcpy(REPLY_START + 14, "\x0\x01\x08\x00\x06\x04\x00\x01", 8) ;

        Spi_Ethernet_memcpy(REPLY_START + 22, macAddr, 6) ;     // mac addr source
        Spi_Ethernet_memcpy(REPLY_START + 28, ipAddr, 4) ;      // ip addr source
        Spi_Ethernet_memcpy(REPLY_START + 32, "\0\0\0\0\0\0", 6) ;      // mac addr destination : broadcast addr
        Spi_Ethernet_memcpy(REPLY_START + 38, ip, 4) ;          // who has this ip addr ?

        /*******************************************************************
         * third, send packet over wires, ARP request packet is 42 bytes len
         */
        Spi_Ethernet_txPacket(42) ;

        tmr = 0 ;
        while(tmr < 45)         // allows around 3 seconds for reply
        {
                Spi_Ethernet_doPacket() ;       // do not stop eth services !

                // select ENC register bank 0, may have been changed by library
                Spi_Ethernet_clearBitReg(ECON1, 0b00000001) ;
                Spi_Ethernet_clearBitReg(ECON1, 0b00000010) ;

                /*
                 * ERDPT[H|L] registers are used by library :
                 * always save them before doing anything else !
                 */
                rdptr = (Spi_Ethernet_readReg(ERDPTH) << 8) + Spi_Ethernet_readReg(ERDPTL) ;

                /*
                 * check if packet is ARP reply to my request (last byte = 2 means ARP reply)
                 */
                if((Spi_Ethernet_memcmp(REPLY_START + 12, "\x08\x06\x00\x01\x08\x00\x06\x04\x00\x02", 10) == 0)
                        && (Spi_Ethernet_memcmp(REPLY_START, macAddr, 6) == 0)  // is it my MAC addr ?
                        && (Spi_Ethernet_memcmp(REPLY_START + 28, ip, 4) == 0)  // is it my IP addr ?
                        )
                        {
                        /*
                         * save remote MAC addr in a free slot of the cache
                         */

                        // copy MAC addr
                        arpCache[freeSlot].mac[0] = Spi_Ethernet_readMem(REPLY_START + 22) ;
                        arpCache[freeSlot].mac[1] = Spi_Ethernet_readMem(REPLY_START + 23) ;
                        arpCache[freeSlot].mac[2] = Spi_Ethernet_readMem(REPLY_START + 24) ;
                        arpCache[freeSlot].mac[3] = Spi_Ethernet_readMem(REPLY_START + 25) ;
                        arpCache[freeSlot].mac[4] = Spi_Ethernet_readMem(REPLY_START + 26) ;
                        arpCache[freeSlot].mac[5] = Spi_Ethernet_readMem(REPLY_START + 27) ;

                        // copy IP addr
                        memcpy(arpCache[freeSlot].ip, ip, 4) ;

                        // enable slot
                        arpCache[freeSlot].valid = 1 ;

                        // timestamp
                        arpCache[freeSlot].tmr = longTmr ;

                        // restore ERDPT before exiting
                        Spi_Ethernet_writeAddr(ERDPTL, rdptr) ;

                        // return the MAC addr pointer
                        return(arpCache[freeSlot].mac) ;
                        }

                // restore ERDPT before doing a new call to the library
                Spi_Ethernet_writeAddr(ERDPTL, rdptr) ;
        }

        // failed, no response : MAC addr is null
        return((unsigned char *)0) ;
}

/*
 * send an UDP packet :
 * destIp       : remote host IP address
 * sourcePort   : local UDP source port number
 * destPort     : destination UDP port number
 * pkt          : pointer to packet to transmit
 * pktLen       : length in bytes of packet to transmit
 */
void sendUDP(unsigned char *destIP, unsigned int sourcePort, unsigned int destPort, unsigned char *pkt, unsigned int pktLen)
{
        unsigned char   *destMac ;      // destination MAC addr
        static int      idUniq ;        // uniq packet id (serial number)
        unsigned char   align ;         // alignement flag
        unsigned int    totlen ;        // total packet length
        unsigned int    rdptr ;         // ENC read pointer backup

        // is destination port valid ?
        if(destPort == 0)
        {
                return ;        // no, do nothing
        }

        // does destination IP address exist ?
        if((destMac = arpResolv(destIP)) == 0)
        {
                return ;        // no MAC addr matches, do nothing
        }

        // select ENC register bank 0
        Spi_Ethernet_clearBitReg(ECON1, 0b00000001) ;
        Spi_Ethernet_clearBitReg(ECON1, 0b00000010) ;

        /*
         * ERDPT[H|L] registers are used by library :
         * always save them before doing anything else !
         */
        rdptr = (Spi_Ethernet_readReg(ERDPTH) << 8) + Spi_Ethernet_readReg(ERDPTL) ;

        /**************************
         * first, build eth header
         */
        Spi_Ethernet_memcpy(REPLY_START, destMAC, 6) ;          // destination
        Spi_Ethernet_memcpy(REPLY_START + 6, macAddr, 6) ;      // source
        Spi_Ethernet_writeMemory(REPLY_START + 12, 8, 0) ;   // IP type

        /**************************
         * second, build IP header
         */
        totlen = 20 + 8 + pktLen ;
        Spi_Ethernet_writeMemory(REPLY_START + 14, 0x45, 0) ;        // IPV4, IHL = 20 bytes, TOS = 0
        Spi_Ethernet_writeMemory(REPLY_START + 16, totlen >> 8, totlen) ;    // total packet length
        Spi_Ethernet_memcpy(REPLY_START + 18, (char *)&(idUniq++), 2) ; // identification
        Spi_Ethernet_writeMemory(REPLY_START + 20, 0, 0) ;           // flags + 0, fragment offset = 0, TTL = 128, protocol = UDP
        Spi_Ethernet_writeMemory(REPLY_START + 22, 0x80, 0x11) ;     // flags + 0, fragment offset = 0, TTL = 128, protocol = UDP
        Spi_Ethernet_writeMemory(REPLY_START + 24, 0, 0) ;   // clear IP checksum
        Spi_Ethernet_memcpy(REPLY_START + 26, ipAddr, 4) ;      // source
        Spi_Ethernet_memcpy(REPLY_START + 30, destIP, 4) ;      // destination

        /***************************
         * third, build UDP header
         */
        totlen = pktLen + 8 ;
        Spi_Ethernet_writeMemory(REPLY_START + 34, sourcePort >> 8, sourcePort) ; // source port
        Spi_Ethernet_writeMemory(REPLY_START + 36, destPort >> 8, destPort) ;   // destination port
        Spi_Ethernet_writeMemory(REPLY_START + 38, totlen >> 8, totlen) ;     // packet length
        Spi_Ethernet_writeMemory(REPLY_START + 40, 0, 0) ;   // clear UDP checksum

        Spi_Ethernet_memcpy(REPLY_START + 42, pkt, pktLen) ;    // payload

        /*****************
         * fourth, IP checksum
         */
        Spi_Ethernet_checksum(REPLY_START + 14, 20) ;
        Spi_Ethernet_writeMemory(REPLY_START + 24, Spi_Ethernet_readReg(EDMACSH), Spi_Ethernet_readReg(EDMACSL)) ;

        /*****************
         * fifth, UDP checksum
         */
        // pseudo header
        // word alignement
        align = totlen & 1 ;
        Spi_Ethernet_writeMem(REPLY_START + 42 + pktLen, 0) ;  // clear

        // build pseudo header
        Spi_Ethernet_RAMcopy(REPLY_START + 26, REPLY_START + 26 + 8, REPLY_START + 42 + pktLen + align, 0) ;
        Spi_Ethernet_writeMemory(REPLY_START + 42 + pktLen + align + 8, 0, 17) ;
        Spi_Ethernet_putByte(totlen >> 8) ;
        Spi_Ethernet_putByte(totlen) ;

        // ready for checksum
        Spi_Ethernet_checksum(REPLY_START + 34, totlen + 12 + align) ;
        Spi_Ethernet_writeMemory(REPLY_START + 40, Spi_Ethernet_readReg(EDMACSH), Spi_Ethernet_readReg(EDMACSL)) ;

        /**************************
         * sixth, send packet over wires
         */
        Spi_Ethernet_txPacket(42 + pktLen) ;

        // restore ERDPT registers before exiting
        Spi_Ethernet_writeAddr(ERDPTL, rdptr) ;
}

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return 0;
}

void Sysclk_Init(void)
{
        OSC_CTRL = 0x02;
        while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
}

void main() {
        Sysclk_Init();
        SPIC_Init();
        PORTC_OUT.B4 = 1; //important to disable SPIC SS prior to configure ethernet
        SPI_Ethernet_Init(macAddr, ipAddr, 0x01);
        SPI_Ethernet_confNetwork("\xFF\xFF\xFF\x00", "\xC0\xA8\x01\x01", "\xC0\xA8\x01\x01");
        
        sendUDP(ntpserver,123,123,
        
        while(1)
                SPI_Ethernet_doPacket();
}