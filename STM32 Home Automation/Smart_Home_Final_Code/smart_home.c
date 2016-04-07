#include "stm32f0xx_rcc.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"
#include "global.h"
#include "pin_conf.h"
#include "stm32f0xx_pwr.h"
#include "stm32f0xx_misc.h"
#include <string.h>
//#include "rad.h"

//#define Tx_mode
#define Rx_mode
unsigned char rf_data[12];
int i;
int iLocal = 0;
unsigned char array[50];
unsigned char * tx_id = "tx-123456";
unsigned char * rx_id = "rx_id-123456";

void device_init(void);
void spi_init(void);
void uart_gpio_init(void);

int main(void)
{
	spi_init();
	setup_rf();																	/* CC2500 RF Module*/
	interrupt_enable();
	uart_gpio_init();																/* Bluetooth*/
	PWR_KEY_OFF;
	AT_KEY_OFF;

	AT_KEY_ON;
	PWR_KEY_ON;

	usart_rxtx(0);																/* 0 for at command enabling*/

	/*	PLACE ALL COMMAND FOR CONFIGURING BLUETOOTH HERE.*/

	PWR_KEY_OFF;
	AT_KEY_OFF;

	usart_rxtx(0);																/* 0 for communication enabling*/

	PWR_KEY_ON;

#ifdef Rx_mode
	cc2500_mode(1); 															/* Configure device in rx/tx mode (1 - rx ,0 - tx)*/
	/*RxData();*/
#else if Tx_mode
			cc2500_mode(0);
#endif
	while (1)
	{

		DEV_1_OFF;
		UARTSend("AT\r\n",sizeof("AT\r\n"));
		delay_ms(1000000);
		//	PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);  //wfi();// commented temporary by rb must remove it.(15/11/14)
		if (request_received)   												/* Must be high on external interrupt*/
		 {
		 request_received = 0;
		 #ifdef Rx_mode
		 RxData();
		 #else if Tx_mode
		 TxData("id+key no which is pressed");
		 #endif
		 }

		/*EXTI_GenerateSWInterrupt(EXTI_Line0);*/
		if (take_action != 0) {
			switch (take_action) {
			case 49:
				DEV_1_TOGGLE;
				/*delay_ms(100);
				led_toggle;*/
			case 50:
				;
			default:
				;
			}
			take_action = 0;
		}
	}
}

void device_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |\
								  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;		/*	Initialize all the connected device*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;									/*	Power supply for Bluetooth*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									/*	AT command key*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void uart_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	/* Configure USART1 Rx (PA.10) as input floating */

	/******************* bluetooth gpio starts ****************************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/******************* bluetooth gpio ends ****************************************/
}
void spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	/* Enable the SPI periph */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 						/* RCC_APB2Periph_SPI1*/

	/* Enable SCK, MOSI and MISO GPIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

	/* Enable CS  GPIO clock */
//	    RCC_AHB1PeriphClockCmd(LIS302DL_SPI_CS_GPIO_CLK, ENABLE);
	/* Enable INT1 GPIO clock */
	//RCC_AHB1PeriphClockCmd(LIS302DL_SPI_INT1_GPIO_CLK, ENABLE);
	/* Enable INT2 GPIO clock */
	//RCC_AHB1PeriphClockCmd(LIS302DL_SPI_INT2_GPIO_CLK, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/******************* SPI configuration *************************************/
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);

	/* Configure GPIO PIN for Lis Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Deselect : Chip Select high */
}

void interrupt_enable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	    /* Configure GPIO PIN for GDO status *///need to configure as an interrupt 28/9/14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 							/* GPIO_PuPd_UP*/

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	EXTI_InitTypeDef EXTI_InitStruct;
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1); 				/* PA1 configured as an interrupt for GDO0*/
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;							/* bluetooth*/
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	   	    /* Deselect : Chip Select high */
//	   	    GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
void USART_Configuration(int baud)
{
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

/* --------------------------------------------------USART1 configuration ------------------------------------------------------*/

  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);


  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  This function handles USARTx global interrupt request
  * @param  None
  * @retval None
  */
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)															/* Loop while there are more characters to send.*/
    {
        USART_SendData(USART1, (uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}
void usart_rxtx(int mode)
{
    const unsigned char welcome_str[] = "AT+ADDR?\r\n";

    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);// | RCC_APB2Periph_GPIOA, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configure the USART1 */
    switch(mode)
    {
    	case 0:
    		USART_Configuration(38400);
    		break;
    	case 1:
    	default:
    		USART_Configuration(9600);
    		break;
    }

    /* Enable the USART1 Receive interrupt: this interrupt is generated when the USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* print welcome information */
    UARTSend(welcome_str, sizeof(welcome_str));

}

void USART1_IRQHandler(void)
{
//	memset(array,0x00,sizeof(array));

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

//		do
//		{
			array[iLocal] = USART_ReceiveData(USART1);
			iLocal++;
			if(iLocal >= 50)
				iLocal = 0;
//				break;
//			delay_ms(10000);
//			/*i = USART_ReceiveData(USART1);*/
//		}
//		while(array[iLocal-1] != '\n');

		if(strcmp(array,"1\r\n") == 0)/*(i == '1')*/
		{
			request_received = 1;
			take_action = i;
			DEV_1_ON;
			//GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);							/* Turn on led connected on PA8*/
			UARTSend("LED ON\r\n",sizeof("LED ON\r\n"));						/* Send acknowledge to Bluetooth device*/
		}
		else if(strcmp(array,"0\r\n") == 0)
		{
			request_received = 1;
			take_action = i;
			DEV_1_OFF;
			//GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);							/* Turn off led connected on PA8*/
			UARTSend("LED OFF\r\n",sizeof("LED OFF\r\n"));						/* Send acknowledge to Bluetooth device*/
		}
		else if(strstr(array,"ERROR:(0)\r\n") != NULL)
		{
			DEV_1_TOGGLE;
			memset(array,0x00,sizeof(array));

		}
		else if(strstr(array,"OK\r\n") != NULL)
		{
			DEV_2_TOGGLE;
			memset(array,0x00,sizeof(array));
		}
	}
}
void EXTI0_1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		char i, k;
		memset(rf_data,0,sizeof(rf_data));										/* clearing the array.*/

		for (i = 0; i < 12; i++) 												/* Reading the data from the fifo*/
				{
			rf_data[i] = Read(CC2500_RXFIFO);
		}
		if (strncmp(rf_data, tx_id, 9) == 0)									/* logic to decode the switch no which is pressed must be here.*/
		{
			take_action = (((rf_data[9] - 0x30) * 10) + (rf_data[10] - 0x30));
			request_received = 1;
			//	send_ack(rx_id);												//commented by ronak 28/9/14 need not to send ack back to master device.
		}
		SendStrobe(CC2500_IDLE);
		SendStrobe(CC2500_FRX);
		SendStrobe(CC2500_RX);
		// Clear the EXTI line 0 pending bit
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
