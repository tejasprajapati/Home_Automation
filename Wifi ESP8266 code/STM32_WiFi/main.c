#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "WiFi_Commands_Prototype.h"
#include <string.h>

char i[500],responce_buffer[200];
int cnt = 0;
//ErrorStatus HSEStartUpStatus;

void usart_rxtx(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
//void UARTSend(char *);
void delay_ms(__IO uint32_t);


int main(void)
{
	char *test;
	usart_rxtx();
	check_working();
	delay_ms(1000000);
	quit_ap();
//	delay_ms(1000);
	query_joined_ap();
    while(1)
    {
    	test = strstr(i,"OK\r\n");
		if(test != NULL)
		{
			cnt = 0;
			//memset(i,0x00,500);
			join_ap("Tejas","7041679923");
		}
	}
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		i[cnt] = USART_ReceiveData(USART1);
		/*if(i == '1'){
			GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);		// Set '1' on PA8
			UARTSend("LED ON\r\n",sizeof("LED ON\r\n"));	// Send message to UART1
		}
		else if(i == '0'){
			GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);		// Set '0' on PA8
			UARTSend("LED OFF\r\n",sizeof("LED OFF\r\n"));
		}*/
		cnt++;
		if(cnt>=500)
			cnt = 0;
	}
}

void usart_rxtx(void)
{
   // const unsigned char welcome_str[] = " Welcome to Bluetooth!\r\n";

    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);// | RCC_APB2Periph_GPIOA, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* NVIC Configuration */
    NVIC_Configuration();

    /* Configure the GPIOs */
    GPIO_Configuration();

    /* Configure the USART1 */
    USART_Configuration();

    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
         USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* print welcome information */
  //  UARTSend(welcome_str, sizeof(welcome_str));
    //UARTSend("AT+CWJAP=\"Tejas\",\"7041679923\"\r\n");

 //   delay_ms(20);

//    UARTSend("AT+CWJAP?\r\n");

//    delay_ms(20);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* Configure (PA.8) as output */
  /*GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure); // Save*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/

  /* Configure USART1 Rx (PA.10) as input floating */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;*/
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1
*******************************************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

/* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = 9600;//115200;		// Baud Rate
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
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USARTx Interrupt */
  /*NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}

/*******************************************************************************
* Function Name  : UARTSend
* Description    : Send a string to the UART.
* Input          : - pucBuffer: buffers to be printed.
*                : - ulCount  : buffer's length
*******************************************************************************/
//void UARTSend(char *pucBuffer)
//{
//    //
//    // Loop while there are more characters to send.
//    //
//    while(*pucBuffer != '\0')
//    {
//        USART_SendData(USART1, (uint16_t) *pucBuffer++);
//        /* Loop until the end of transmission */
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//        {
//        }
//    }
//}


//void delay_ms(__IO uint32_t nTime)
//{
//	__IO uint32_t TimingDelay = 0;
//	TimingDelay = nTime;
//	for(TimingDelay=0;TimingDelay<nTime;TimingDelay++);
//	//while (TimingDelay != 0)
//	//	;
//}
