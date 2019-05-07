// PN5180 1-Byte Direct Commands
// see 11.4.3.3 Host Interface Command List
#define PN5180_WRITE_REGISTER           (0x00)
#define PN5180_WRITE_REGISTER_OR_MASK   (0x01)
#define PN5180_WRITE_REGISTER_AND_MASK  (0x02)
#define PN5180_READ_REGISTER            (0x04)
#define PN5180_READ_EEPROM              (0x07)
#define PN5180_SEND_DATA                (0x09)
#define PN5180_READ_DATA                (0x0A)
#define PN5180_SWITCH_MODE              (0x0B)
#define PN5180_LOAD_RF_CONFIG           (0x11)
#define PN5180_RF_ON                    (0x16)
#define PN5180_RF_OFF (0x17)

// PN5180 Registers
#define SYSTEM_CONFIG       (0x00)
#define IRQ_ENABLE          (0x01)
#define IRQ_STATUS          (0x02)
#define IRQ_CLEAR           (0x03)
#define TRANSCEIVE_CONTROL  (0x04)
#define TIMER1_RELOAD       (0x0c)
#define TIMER1_CONFIG       (0x0f)
#define RX_WAIT_CONFIG      (0x11)
#define CRC_RX_CONFIG       (0x12)
#define RX_STATUS           (0x13)
#define RF_STATUS           (0x1d)
#define SYSTEM_STATUS       (0x24)
#define TEMP_CONTROL        (0x25)

// PN5180 EEPROM Addresses
#define DIE_IDENTIFIER      (0x00)
#define PRODUCT_VERSION     (0x10)
#define FIRMWARE_VERSION    (0x12)
#define EEPROM_VERSION      (0x14)
#define IRQ_PIN_CONFIG (0x1A)

// PN5180 IRQ_STATUS
#define RX_IRQ_STAT         (1<<0)  // End of RF rececption IRQ
#define TX_IRQ_STAT         (1<<1)  // End of RF transmission IRQ
#define IDLE_IRQ_STAT       (1<<2)  // IDLE IRQ
#define RFOFF_DET_IRQ_STAT  (1<<6)  // RF Field OFF detection IRQ
#define RFON_DET_IRQ_STAT   (1<<7)  // RF Field ON detection IRQ
#define TX_RFOFF_IRQ_STAT   (1<<8)  // RF Field OFF in PCD IRQ
#define TX_RFON_IRQ_STAT    (1<<9)  // RF Field ON in PCD IRQ
#define RX_SOF_DET_IRQ_STAT (1<<14) // RF SOF Detection IRQ
