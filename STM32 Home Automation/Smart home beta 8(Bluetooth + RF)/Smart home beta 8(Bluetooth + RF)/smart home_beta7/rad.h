#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
/*
#define SS_T TRISBbits.TRISB2
#define SS PORTBbits.RB2//as connect to CS of CC pin//SS1
#define GDO_T TRISAbits.TRISA14
#define GDO PORTAbits.RA14//as connect to GDO of CC pin//J12 - 10
#define SCK_T TRISDbits.TRISD10
#define SCK PORTDbits.RD10//as connect to SCK of CC pin//SCK1
#define MISO_T TRISCbits.TRISC4
#define MISO PORTCbits.RC4//as connect to SO of CC pin//SDI1
#define MOSI_T TRISDbits.TRISD0
#define MOSI PORTDbits.RD0//as connect to SI of CC pin//SDO1
*/

//#define Tx_mode
//#define Rx_mode

//unsigned char * tx_id = "tx-123456";
//unsigned char * rx_id = "rx_id-123456";

#define SS GPIO_Pin_13;
#define GDO GPIO_Pin_2;
#define MISO GPIO_Pin_6;
#define MOSI GPIO_Pin_7;

//void WriteReg(char add,char data);
//char ReadReg(char add);
//char Read(char add);
//char SendStrobe(char strobe);
//void init_CC2500(void);
//void Read_Config_Regs(void);
//void setup(void);
//void cc2500_mode(char);
//void TxData(unsigned char*);
//void RxData(void);

/*****************************************************************/

//#define CC2500_IDLE    0x36      // Exit RX / TX, turn
//#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
//#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
//#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
//#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
//#define CC2500_TXFIFO  0x3F		 // TX FIFO BUFFER
//#define CC2500_RXFIFO  0x3F      // RX FIFO BUFFER


/***************************************************************/

#define REG_IOCFG2           0x0000
#define REG_IOCFG1           0x0001
#define REG_IOCFG0           0x0002
#define REG_FIFOTHR          0x0003
#define REG_SYNC1            0x0004
#define REG_SYNC0            0x0005
#define REG_PKTLEN           0x0006
#define REG_PKTCTRL1         0x0007
#define REG_PKTCTRL0         0x0008
#define REG_ADDR             0x0009
#define REG_CHANNR           0x000A
#define REG_FSCTRL1          0x000B
#define REG_FSCTRL0          0x000C
#define REG_FREQ2            0x000D
#define REG_FREQ1            0x000E
#define REG_FREQ0            0x000F
#define REG_MDMCFG4          0x0010
#define REG_MDMCFG3          0x0011
#define REG_MDMCFG2          0x0012
#define REG_MDMCFG1          0x0013
#define REG_MDMCFG0          0x0014
#define REG_DEVIATN          0x0015
#define REG_MCSM2            0x0016
#define REG_MCSM1            0x0017
#define REG_MCSM0            0x0018
#define REG_FOCCFG           0x0019
#define REG_BSCFG            0x001A
#define REG_AGCCTRL2         0x001B
#define REG_AGCCTRL1         0x001C
#define REG_AGCCTRL0         0x001D
#define REG_WOREVT1          0x001E
#define REG_WOREVT0          0x001F
#define REG_WORCTRL          0x0020
#define REG_FREND1           0x0021
#define REG_FREND0           0x0022
#define REG_FSCAL3           0x0023
#define REG_FSCAL2           0x0024
#define REG_FSCAL1           0x0025
#define REG_FSCAL0           0x0026
#define REG_RCCTRL1          0x0027
#define REG_RCCTRL0          0x0028
#define REG_FSTEST           0x0029
#define REG_PTEST            0x002A
#define REG_AGCTEST          0x002B
#define REG_TEST2            0x002C
#define REG_TEST1            0x002D
#define REG_TEST0            0x002E
#define REG_PARTNUM          0x0030
#define REG_VERSION          0x0031
#define REG_FREQEST          0x0032
#define REG_LQI              0x0033
#define REG_RSSI             0x0034
#define REG_MARCSTATE        0x0035
#define REG_WORTIME1         0x0036
#define REG_WORTIME0         0x0037
#define REG_PKTSTATUS        0x0038
#define REG_VCO_VC_DAC       0x0039
#define REG_TXBYTES          0x003A
#define REG_RXBYTES          0x003B
#define REG_RCCTRL1_STATUS   0x003C
#define REG_RCCTRL0_STATUS   0x003D


/****************************************************************************

/* Sync word qualifier mode = 30/32 sync word bits detected */

/* Packet length mode = Fix packet length mode. Packet length configured by the first byte after sync word */

/* Whitening = false */

/* Data format = Normal mode */

/* Address config = No address check */

/* Channel spacing = 199.951172 *///if 26MHz then 8 gives 199.95k spacing (IF)

/* Carrier frequency = 2432.999908M*///5D93B1=6132657. Fc=(Fx/2^16)*FREQ;this value at 26MHz

/* Base frequency = 2432.999908M*/

/* RX filter BW = 203.125000k *///for 26MHz

/* Data rate = 2.39897k *///0x83=131d

/* Deviation = 38.085938k*/// at 26MHz

/* Device address = 0 *///AUTOMATICALY CALIBERATE WHEN GOING FROM IDLE TO TX/RX AND EXPIRY 64 COUNT

/* CRC autoflush = false */

/* Preamble count = 4 */

/* Manchester enable = false */

/* CRC enable = true */

/* Packet length = 6 */

/* Modulation format = 2-FSK */

/* Modulated = true */

/* Channel number = 0 */

/* {TX power = 0 }*/

/* PA table */

#define PA_TABLE {0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

/****************************************************************************



/**************************************************************************/
#define VAL_IOCFG2           0x01//0x29//default chip ready   added by ronak for interrupt driven solution testing phase (28/9/14)
#define VAL_IOCFG1           0x2E//default high impedence
#define VAL_IOCFG0           0x06//Asserts when word has sent/receive,de-asserts at end.In RX, pin will de-assert when address check fails or  RX FIFO overflows. In TX pin will de-assert if TX FIFO underflows.
#define VAL_FIFOTHR          0x07//33/32 in TX/RX
#define VAL_SYNC1            0xD3//default 8 MSB
#define VAL_SYNC0            0x91//default 8 LSB
#define VAL_PKTLEN           0x0C//6 length packet
#define VAL_PKTCTRL1         0x04//no addres check
#define VAL_PKTCTRL0         0x04//whitening off,CRC on, fix packet
#define VAL_ADDR             0x00//default no address
#define VAL_CHANNR           0x00//default channel no 0
#define VAL_FSCTRL1          0x08//IF freq 199.95
#define VAL_FSCTRL0          0x00//default freq offset
#define VAL_FREQ2            0x5D//carrier freq 2432.M  5D93B1=6132657.
#define VAL_FREQ1            0x93//carrier freq 2432.M
#define VAL_FREQ0            0xB1//carrier freq 2432.M
#define VAL_MDMCFG4          0x86//BW 203k 0x8E
#define VAL_MDMCFG3          0x83//data rate 2.3989k change to 500.0 k baud rate 0x3B
#define VAL_MDMCFG2          0x03//30/32 detect 2-FSK
#define VAL_MDMCFG1          0x22//default 4 preamble
#define VAL_MDMCFG0          0xF8//default 199.95 spacing
#define VAL_DEVIATN          0x44//deviation 38.085k
#define VAL_MCSM2            0x07//default time out untill packet end
#define VAL_MCSM1            0x2F//0x30//default If RSSI below threshold unless currently receiving packet
#define VAL_MCSM0            0x18//64 expire , IDLE to TX/RX
#define VAL_FOCCFG           0x16//gate disable
#define VAL_BSCFG            0x6c//default
#define VAL_AGCCTRL2         0x03//default 33 db
#define VAL_AGCCTRL1         0x40//default relative carrier sense disable
#define VAL_AGCCTRL0         0x91//default normal operation, channel filter sample 16
#define VAL_WOREVT1          0x87//default 876B=34667 appx. 1 s time out
#define VAL_WOREVT0          0x6B//default 1 s time out
#define VAL_WORCTRL          0xF8//default RC calibration on
#define VAL_FREND1           0x56//default
#define VAL_FREND0           0x10//default
#define VAL_FSCAL3           0xA9//default
#define VAL_FSCAL2           0x0A//default
#define VAL_FSCAL1           0x00//
#define VAL_FSCAL0           0x11//
#define VAL_RCCTRL1          0x41//default
#define VAL_RCCTRL0          0x00//default
#define VAL_FSTEST           0x59//default
#define VAL_PTEST            0x7F//default
#define VAL_AGCTEST          0x3F//default
#define VAL_TEST2            0x88//default
#define VAL_TEST1            0x31//default
#define VAL_TEST0            0x0B//default
/*********************************************************************/
#define VAL_PARTNUM          0x80//default
#define VAL_VERSION          0x03//default
#define VAL_FREQEST          0x00//default
#define VAL_LQI              0x00//default
#define VAL_RSSI             0x00//default
#define VAL_MARCSTATE        0x00//default
#define VAL_WORTIME1         0x00//default
#define VAL_WORTIME0         0x00//default
#define VAL_PKTSTATUS        0x00//default
#define VAL_VCO_VC_DAC       0x00//default
#define VAL_TXBYTES          0x00//default
#define VAL_RXBYTES          0x00//default
#define VAL_RCCTRL1_STATUS   0x00//default
#define VAL_RCCTRL0_STATUS   0x00//default


/*************************************************************************************/
