#include "rad.h"
#include "global.h"
#include "stm32f0xx_gpio.h"
#include"stm32f0xx_spi.h"
unsigned char response_aray[100];
void delay_ms(__IO uint32_t nTime) {
	__IO uint32_t TimingDelay = 0;
	TimingDelay = nTime;
	for(TimingDelay=0;TimingDelay<nTime;TimingDelay++);
	//while (TimingDelay != 0)
	//	;
}
char Read(char add)
{
	unsigned char x, y;
	add = add + 0x80; //MSB OF ADR IS HIGH FOR READ

	GPIOA->ODR &=(0<<3);    //lo
//	while(GPIOA->IDR &=(1<<6));
	//while(MISO);
	SPI_SendData8(SPI1, add); // putcSPI1(add);         			//SEND ADR BYTE
	y = SPI_ReceiveData8(SPI1);

	SPI_SendData8(SPI1, 0); //SEND PROXY DATA
	y = SPI_ReceiveData8(SPI1); //getcSPI1();

	GPIOA->ODR |= (1<<3);	// hi
	return y;
}

void WriteReg(char add, char data) {
	unsigned char x, y;
	GPIOA->ODR &=(0<<3);    //lo
//	while(GPIOA->IDR &=(1<<6));
	SPI_SendData8(SPI1, add); //SEND ADR BYTE
	y = SPI_ReceiveData8(SPI1); //getcSPI1();

	SPI_SendData8(SPI1, data); //SEND DATA BYTE
	y = SPI_ReceiveData8(SPI1); //getcSPI1();

	GPIOA->ODR |= (1<<3);	// hi
}
char SendStrobe(char strobe)
{
	unsigned char out;
	GPIOA->ODR &=(0<<3);    //lo
//	while(GPIOA->IDR &=(1<<6));
	SPI_SendData8(SPI1, strobe); //SEND STROBE BYTE
	out = SPI_ReceiveData8(SPI1); //getcSPI1();
	GPIOA->ODR |= (1<<3);	// hi
	return out;
}
//void loop_T() {
//	delay_ms(10);
//	TxData(tx_id); // by ronak data id send
//	delay_ms(10);
//}

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
//void loop_R()
//{
////  Read_Config_Regs();
//	RxData();
//}

void TxData(unsigned char* data)
{
	unsigned char data2[12], i,length =12;//="temp must be changed"
	strcpy(data2,data);

	// Make sure that the radio is in IDLE state before flushing the FIFO
	SendStrobe(CC2500_IDLE);
	delay_ms(10);
	// Flush TX FIFO
	SendStrobe(CC2500_FTX);
	delay_ms(10);

	// SIDLE: exit RX/TX
	SendStrobe(CC2500_IDLE);
	delay_ms(10);

	for (i = 0; i < length; i++) {
		WriteReg(CC2500_TXFIFO, data2[i]);
	}
	// STX: enable TX
	SendStrobe(CC2500_TX);
	delay_ms(10);

	// Wait for GDO0 to be set -> sync transmitted
	while(!(GPIOA->IDR & GPIO_Pin_1));
//	while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));

//      while (!GDO);

	// Wait for GDO0 to be cleared -> end of packet
		while(GPIOA->IDR & GPIO_Pin_1);
//	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));

//       while (GDO);

//lcd_data('k');
} // Rf TX Packet

void RxData(void)
{
	unsigned char data[14];
	char i, k;
	unsigned long int tag = 0;

	for (k = 0; k < 14; k++) {
		data[k] = '\0';
	} //CLEARING ARRAY

	// RX: enable RX
//	SendStrobe(CC2500_RX);		// commented by ronak for smart home .

//***********************************************************************************************

	// Wait for GDO0 to be set -> sync received
	//    while (!GDO)
	//while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))


	while(!(GPIOA->IDR &(1<<1)))			// commented by ronak for smart home .
	{							//	while(!(GPIOA->IDR & GPIO_Pin_3))
		tag++;
		if (tag > 65000000)
		{
			tag = 0;
			break;
		}
	}


	//***********************************************************************************************
//    // Wait for GDO0 to be cleared -> end of packet

//    while (GDO)
//	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
//	while((GPIOA->IDR & GPIO_Pin_3))


	while(GPIOA->IDR &(1<<1))			// commented by ronak for smart home .
	{
		tag++;
		if (tag > 65000000)
		{
			tag = 0;
			break;
		}
	}


	// Read data from RX FIFO and store in rxBuffer
	for (i = 0; i < 12; i++) {
		data[i] = Read(CC2500_RXFIFO);
	}

	if(strncmp(data,tx_id,9)==0)
	{			// logic to decode the switch no which is pressed must be here.

		take_action = (((data[9]-0x30)*10) + (data[10]-0x30));
//		send_ack(rx_id);			//commented by ronak 28/9/14 need not to send ack back to master device.
	}
	SendStrobe(CC2500_IDLE);
	delay_ms(10);
	// Flush RX FIFO
	SendStrobe(CC2500_FRX);
	delay_ms(10);


//lcd_str(data);
//	tag = 0;		// commented by ronak for smart home .
} // Rf RxPacket
void init_CC2500(void) {
//	WriteReg(REG_IOCFG2, VAL_IOCFG2);  // commented by rb for testing without interrupt.(15/11/14)
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
void setup(void) {
//	SS_T=0;					//SS AS OUTPUT
	GPIOA->BSRR = SS;
//	SS=1;
//    GDO_T=1;				//GDO AS INPUT
	Read_Config_Regs();
	delay_ms(1000);
	init_CC2500();
	delay_ms(1000);
}
