#include "misc.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

#define DST_IP "188.226.224.148" //api.openweathermap.org 's IP Address
#define LOCATIONID "2925533" // Weather City ID

void GPIO_configuration(void);
void USART2_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer);
void delay(unsigned int);
void flush_response(void);
char response[250],index_array,command_mode,local_ip[20],idx,*url= "GET / HTTP/1.0\r\n\r\n" ;    //"GET /data/2.5/weather?id=2925533 HTTP/1.0\r\nHost: api.openweathermap.org\r\n\r\n";
int i,len;

int main(void)
{
	GPIO_configuration();
	check_working();		// dummy call to remove error
	while(1)
	{
	disable_echo();
	flush_response();
	check_working();
	flush_response();
//	restart();
//	flush_response();
	firmware_version();//version 20000903 15th december 2014
	flush_response();
	UARTSend("AT+CWMODE=3"); // 3 for use as ap and sta both .for testing if not working remove it.
	delay_ms(2000);
	flush_response();
	query_wifi_mode();
	flush_response();
//	wifi_mode('1');		// uncommented by ronak reference form sunrom.com docs.
//	flush_response();
//	restart();
//	flush_response();
	query_joined_ap();
	flush_response();
	list_ap();
	flush_response();
	join_ap("Ronak","rbpanchal123");
	flush_response();
	get_ip_addr();
	flush_response();
	connection_multiple(1);
	flush_response();
//	query_connection();	 commented on 6/12/15
//	flush_response();

//	connection_multiple('0');
//	flush_response();


	UARTSend("AT+CIPSTART=\"TCP\",\"google.com\",80\r\n"); // working ok verified by ronak

//	UARTSend("AT+CIPSTART=\"TCP\",\"188.226.224.148\",80\r\n");    // 6/12/15 // error of entry point no working

//	UARTSend(DST_IP);
//	UARTSend("\",80\r\n");

	delay_ms(2000);
//	set_connection("0","TCP","google.com","80");		// function definition has error need to change as per above.
	flush_response();
	len = strlen(url);
	UARTSend("AT+CIPSEND=");//modified added 4 ahead 6/12/15
	UARTSend(len);
	UARTSend("\r\n");

//	UARTSend("AT+CIPSEND=34\r\n");		// working ok verified by ronak
	delay_ms(5000);
	if(strstr(response,">")!= "\0")
	{
//		UARTSend("GET / HTTP/1.0\r\n\r\n+++");	// working ok verified by ronak
		UARTSend("GET / HTTP/1.0\r\nHost: www.yahoo.com\r\n\r\n");  // testing
	//	delay_ms(10);
	//	UARTSend("\r\n");
		delay_ms(10000);
		while(strncmp(response,"+IPD",4));
	}
	flush_response();
	close_connection();	// not verified yet need to check
	flush_response();
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//	GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
//	GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_SET);
//	command_mode = 1;
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
//		delay(99999);

//	UARTSend("AT\r\n");  //test command
//	delay(99999);
/*	UARTSend("AT+VERSION?\r\n");  //test VERSION 2.0
		delay(99999);
//	UARTSend("AT+ADDR?\r\n");  //test
		delay(99999);
	UARTSend("AT+NAME=RONAK\r\n");  //test command
		delay(99999);
/*	UARTSend("AT+STATE?\r\n");  //test command
		delay(99999);

	UARTSend("AT+CMODE=1\r\n");
		delay(99999);
	UARTSend("AT+ROLE=0\r\n");
		delay(99999);
//	UARTSend("AT+ROLE?\r\n");
//		delay(99999);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	command_mode = 0;
//	UARTSend("Hello World....!!!!\r\n");*/
	}
	while(1)
    {
    }

}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
//		if(command_mode)
//		{
			response[index_array++] = USART_ReceiveData(USART2);
//			if(index_array>=49)index_array = 0;
//		}

//		i = USART_ReceiveData(USART2);
//		if(i == '1'){
//			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);		// Set '1' on PA8
//			GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_RESET);
//			UARTSend(" 1ST LED ON\r\n");
//		}
//		else if(i == '0'){
//			GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_SET);
//			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_RESET);		// Set '0' on PA8
//			UARTSend(" 2ND LED ON\r\n");
//		}
	}
}

void UARTSend(const unsigned char *pucBuffer)
{
    //
    // Loop while there are more characters to send.
    //
    while(*pucBuffer!= '\0')
    {
        USART_SendData(USART2, (uint16_t) *pucBuffer);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        }
        *pucBuffer++;
    }
}

void GPIO_configuration(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity =    USART_Parity_No;
	USART_InitStruct.USART_StopBits =  USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2,&USART_InitStruct);

	USART_ClockInitTypeDef USART_ClockInitStruct;
	USART_ClockInitStruct.USART_Clock = USART_Clock_Enable;
	USART_ClockStructInit(&USART_ClockInitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART2, USART_IT_TXE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}
void delay_ms(unsigned int count)
{
	unsigned int j = 0,k=0;
	for(j = 0;j<=count;j++)
	{
		for(k=0;k<=1000;k++)
		{
			__NOP();
			__NOP();
		}
	}
}
void flush_response(void)
{
	memset(response,0x00,sizeof(response));
	index_array =0;
}
