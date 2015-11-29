#include "rad.h"
#include "stm8l15x_spi.h"
#include "string.h"

extern unsigned char *tx_id;
extern unsigned char *rx_id;
unsigned char response_aray[65];

void delay_ms(__IO uint32_t nTime) 
{
  __IO uint32_t TimingDelay = 0;
  TimingDelay = nTime;
  for(TimingDelay=0;TimingDelay<nTime;TimingDelay++);
}
char Read(char add)
{
	unsigned char x, y;
	add = add + 0x80; //MSB OF ADR IS HIGH FOR READ

	ss_low;    //lo

        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
	SPI_SendData(SPI1, add); // putcSPI1(add);         			//SEND ADR BYTE
        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	y = SPI_ReceiveData(SPI1);

        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
	SPI_SendData(SPI1, 0); //SEND PROXY DATA
        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	y = SPI_ReceiveData(SPI1); //getcSPI1();

	ss_high;	// hi
	return y;
}

void WriteReg(char add, char data) 
{
	unsigned char x, y;
	ss_low;    //lo

        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
	SPI_SendData(SPI1, add); // putcSPI1(add);         			//SEND ADR BYTE
        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	y = SPI_ReceiveData(SPI1);

        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
	SPI_SendData(SPI1, data); //SEND PROXY DATA
        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	y = SPI_ReceiveData(SPI1); //getcSPI1();

	ss_high;	// hi
}
  
char SendStrobe(char strobe) 
{
	unsigned char out;
	ss_low;    //lo

        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
	SPI_SendData(SPI1, strobe); //SEND STROBE BYTE
        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	out = SPI_ReceiveData(SPI1); 

	ss_high;	// hi
	return out;
}
void cc2500_mode(char mode)
{
	if(mode==1)
	{
		SendStrobe(CC2500_RX);				//	RX: enable RX
	}
	else if(mode == 0)
	{
	//	SendStrobe(CC2500_TX);				// TX mode enable (need to think about logic)
		SendStrobe(CC2500_IDLE);
	}
}
//void loop_T() 
//{
//	delay_ms(10);
//	TxData(6); //  Transmit No_of_Bytes
//	delay_ms(10);
//}
//
//void loop_R() {
//        Read_Config_Regs();
//	RxData();
//}

void TxData(unsigned char *data) 
{
    unsigned char data2[12], i,length =12;//="temp must be changed"
	strcpy(data2,data);
//	unsigned char data2[6] = "techno", i;

	SendStrobe(CC2500_IDLE);        // Make sure that the radio is in IDLE state before flushing the FIFO
	SendStrobe(CC2500_FTX);         // Flush TX FIFO
	SendStrobe(CC2500_IDLE);	// SIDLE: exit RX/TX
	for (i = 0; i < length; i++) {
		WriteReg(CC2500_TXFIFO, data2[i]);
	}
	SendStrobe(CC2500_TX);          // STX: enable TX
	while(!(GPIOA->IDR & GPIO_Pin_3));      // Wait for GDO0 to be set -> sync transmitted	
//      while (!GDO);

	while(GPIOA->IDR & GPIO_Pin_3);         // Wait for GDO0 to be cleared -> end of packet	
//       while (GDO);
        
//         RxData();              // commented by ronak 22/10/14 (no ack required)
} // Rf TX Packet

char RxData(void) 
{
        unsigned char ack = 0;
	unsigned char data[12];
	char i, k;
	unsigned long int tag = 0;

	for (k = 0; k < 12; k++) {
		data[k] = '\0';
	} //CLEARING ARRAY

	// RX: enable RX
	SendStrobe(CC2500_RX);

	// Wait for GDO0 to be set -> sync received
	//    while (!GDO)
	//while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
	while(GPIOA->IDR &=(1<<3))
//	while(!(GPIOA->IDR & GPIO_Pin_3))
	{
		tag++;
		if (tag > 6500000)
		{
			tag = 0;
			break;
		}
	}

//    // Wait for GDO0 to be cleared -> end of packet

//    while (GDO)
//	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
//	while((GPIOA->IDR & GPIO_Pin_3))
	while(GPIOA->IDR &=(1<<3))
	{
		tag++;
		if (tag > 6500000)
		{
			tag = 0;
			break;
		}
	}

	// Read data from RX FIFO and store in rxBuffer
	for (i = 0; i < 6; i++) {
		data[i] = Read(CC2500_RXFIFO);
	}

	SendStrobe(CC2500_IDLE);
	// Flush RX FIFO
	SendStrobe(CC2500_FRX);
        if(strcmp(data,rx_id)==0)
        {
          ack = 1;
        }
	tag = 0;
        return ack;
} // Rf RxPacket
void init_CC2500(void) 
{
        Read_Config_Regs();
	WriteReg(REG_IOCFG2, VAL_IOCFG2);
	WriteReg(REG_IOCFG1, VAL_IOCFG1);
	WriteReg(REG_IOCFG0, VAL_IOCFG0);
	WriteReg(REG_FIFOTHR, VAL_FIFOTHR);
	WriteReg(REG_SYNC1, VAL_SYNC1);
	WriteReg(REG_SYNC0, VAL_SYNC0);
	WriteReg(REG_PKTLEN, VAL_PKTLEN);
	WriteReg(REG_PKTCTRL1, VAL_PKTCTRL1);
	WriteReg(REG_PKTCTRL0, VAL_PKTCTRL0);
	WriteReg(REG_ADDR, VAL_ADDR);
	WriteReg(REG_CHANNR, VAL_CHANNR);
	WriteReg(REG_FSCTRL1, VAL_FSCTRL1);
	WriteReg(REG_FSCTRL0, VAL_FSCTRL0);
	WriteReg(REG_FREQ2, VAL_FREQ2);
	WriteReg(REG_FREQ1, VAL_FREQ1);
	WriteReg(REG_FREQ0, VAL_FREQ0);
	WriteReg(REG_MDMCFG4, VAL_MDMCFG4);
	WriteReg(REG_MDMCFG3, VAL_MDMCFG3);
	WriteReg(REG_MDMCFG2, VAL_MDMCFG2);
	WriteReg(REG_MDMCFG1, VAL_MDMCFG1);
	WriteReg(REG_MDMCFG0, VAL_MDMCFG0);
	WriteReg(REG_DEVIATN, VAL_DEVIATN);
	WriteReg(REG_MCSM2, VAL_MCSM2);
	WriteReg(REG_MCSM1, VAL_MCSM1);
	WriteReg(REG_MCSM0, VAL_MCSM0);
	WriteReg(REG_FOCCFG, VAL_FOCCFG);

	WriteReg(REG_BSCFG, VAL_BSCFG);
	WriteReg(REG_AGCCTRL2, VAL_AGCCTRL2);
	WriteReg(REG_AGCCTRL1, VAL_AGCCTRL1);
	WriteReg(REG_AGCCTRL0, VAL_AGCCTRL0);
	WriteReg(REG_WOREVT1, VAL_WOREVT1);
	WriteReg(REG_WOREVT0, VAL_WOREVT0);
	WriteReg(REG_WORCTRL, VAL_WORCTRL);
	WriteReg(REG_FREND1, VAL_FREND1);
	WriteReg(REG_FREND0, VAL_FREND0);
	WriteReg(REG_FSCAL3, VAL_FSCAL3);
	WriteReg(REG_FSCAL2, VAL_FSCAL2);
	WriteReg(REG_FSCAL1, VAL_FSCAL1);
	WriteReg(REG_FSCAL0, VAL_FSCAL0);
	WriteReg(REG_RCCTRL1, VAL_RCCTRL1);
	WriteReg(REG_RCCTRL0, VAL_RCCTRL0);
	WriteReg(REG_FSTEST, VAL_FSTEST);
	WriteReg(REG_PTEST, VAL_PTEST);
	WriteReg(REG_AGCTEST, VAL_AGCTEST);
	WriteReg(REG_TEST2, VAL_TEST2);
	WriteReg(REG_TEST1, VAL_TEST1);
	WriteReg(REG_TEST0, VAL_TEST0);
	/*
	 WriteReg(REG_PARTNUM,VAL_PARTNUM);
	 WriteReg(REG_VERSION,VAL_VERSION);
	 WriteReg(REG_FREQEST,VAL_FREQEST);
	 WriteReg(REG_LQI,VAL_LQI);
	 WriteReg(REG_RSSI,VAL_RSSI);
	 WriteReg(REG_MARCSTATE,VAL_MARCSTATE);
	 WriteReg(REG_WORTIME1,VAL_WORTIME1);
	 WriteReg(REG_WORTIME0,VAL_WORTIME0);
	 WriteReg(REG_PKTSTATUS,VAL_PKTSTATUS);
	 WriteReg(REG_VCO_VC_DAC,VAL_VCO_VC_DAC);
	 WriteReg(REG_TXBYTES,VAL_TXBYTES);
	 WriteReg(REG_RXBYTES,VAL_RXBYTES);
	 WriteReg(REG_RCCTRL1_STATUS,VAL_RCCTRL1_STATUS);
	 WriteReg(REG_RCCTRL0_STATUS,VAL_RCCTRL0_STATUS);
	 */
}
void Read_Config_Regs(void)
{
	unsigned int addr = 0,inc=0;
	for(inc=0;inc<99;inc++)
	{
		response_aray[inc]=Read(addr++);
	}
}
void setup(void) 
{
	ss_high;
//	SS=1;

	delay_ms(1000);
	init_CC2500();
//        Read_Config_Regs();
	delay_ms(1000);
}
