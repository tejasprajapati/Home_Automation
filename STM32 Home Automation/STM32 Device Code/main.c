#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include"stm32f0xx_spi.h"
SPI_InitTypeDef SPI_InitStruct;
GPIO_InitTypeDef GPIO_InitStruct;
void setup(void);
void loop_T(void);
void loop_R(void);
//void spi_init(void) {
//	RCC->APB2ENR |=(1<<12); // SPI CLK ENABLE
//	SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; /* (1) */
//	SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_FRXTH
//			| SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; /* (2) */
//			SPI1->CR1 |= SPI_CR1_SPE; /* (3) */
//}
void io_init(void) {
	GPIO_DeInit(GPIOA);
	RCC->AHBENR|=(1<<17);  // PORTA CLK ENABLED
	GPIO_PinAFConfig(GPIOA,GPIO_Pin_5,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_Pin_6,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_Pin_7,GPIO_AF_0);
	GPIO_StructInit(&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13; // for cs pin
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

//	GPIO_DeInit(GPIOC);
//	GPIO_StructInit(&GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // for cs pin
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStruct);
// by default every pin is configured as a input.
	/*	GPIO_PinLockConfig(GPIOA,GPIO_Pin_2);//gdo1
	 GPIO_PinLockConfig(GPIOA,GPIO_Pin_3);//gdo0
	 GPIO_PinLockConfig(GPIOA,GPIO_Pin_4);//cs
	 GPIO_PinLockConfig(GPIOA,GPIO_Pin_6);//miso
	 */
//	RCC->AHBENR|=(1<<17);  // PORTA CLK ENABLED
//	GPIOA->MODER|=(1<<8);  // O/P MODE
//	GPIOA->ODR |= (1<<4);	// hi
//	GPIOA->ODR &=(0<<4);    // lo
}
void spi_init(void) {
	SPI_I2S_DeInit(SPI1);
	RCC->APB2ENR |=(1<<12); // SPI CLK ENABLE
	RCC_AHBPeriphClockCmd();
	io_init();

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	/* Initialize the SPI_Mode member */
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	/* Initialize the SPI_DataSize member */
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	/* Initialize the SPI_CPOL member */
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_Low;//
	/* Initialize the SPI_CPHA member */
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	/* Initialize the SPI_NSS member */
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	/* Initialize the SPI_FirstBit member */
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	/* Initialize the SPI_CRCPolynomial member */
	SPI_InitStruct.SPI_CRCPolynomial = 7;
//	  SPI_StructInit(&spi1_structure);
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}


//unsigned char response_aray[100];
int main(void) {
/*
	RCC->AHBENR|=(1<<17);  // PORTA CLK ENABLED
	RCC->AHBENR |= (1<<19); // CLK ENBLE portc
	GPIOA->MODER|=(1<<8);  // O/P MODE
	GPIOA->ODR |= (1<<4);	// ON LED
	GPIOA->ODR &=(0<<4);    // OFF LED WORKING LED CODE BY RB

	RCC->AHBENR |= (1<<18); // CLK ENBLE portc
		GPIOB->MODER|=(1<<8);  // O/P MODE  // 8 means 4 the pin as a o/p
		GPIOB->ODR |= (1<<4);	// ON LED
		GPIOB->ODR &=(0<<4);    // OFF LED WORKING LED CODE BY RB

	//GPIOA->MODER  // NOT NEEDED OF USING IT FOR I/P
//	while(1)
//	{
	if(GPIOA->IDR &=(1<<0))   // user button sensing verified by rb working ok
	{
		RCC->AHBENR |= (1<<19); // CLK ENBLE portc
		GPIOC->MODER|=(1<<16);  // O/P MODE
		GPIOC->ODR |= (1<<8);	// ON LED
//		GPIOC->ODR &=(0<<8);    // OFF LED WORKING LED CODE BY RB
//		GPIOC->ODR &=(0<<9);
	}
	else
	{
		RCC->AHBENR |= (1<<19); // CLK ENBLE portc
		GPIOC->MODER|=(1<<18);  // O/P MODE
		GPIOC->ODR |= (1<<9);	// ON LED
//		GPIOC->ODR &=(0<<9);    // OFF LED WORKING LED CODE BY RB
//		GPIOC->ODR &=(0<<8);
	}
//	}


	RCC->APB2ENR |=(1<<12); // SPI CLK ENABLE
	GPIOA->MODER |=(1<<9); // CS PIN AS A O/P DEFINED OTHERS AS A I/P BY DEFAULT


	RCC->AHBENR|=(1<<17);  // PORTA CLK ENABLED
	GPIOA->MODER|=(1<<8);  // O/P MODE
	GPIOA->ODR |= (1<<4);	// high
	GPIOA->ODR &= ~(1<<4);	// low
	*/
	spi_init();
	io_init();
	setup();

	while (1) {
		loop_T();
		loop_R();
		/*   	 unsigned char data =0 ;
		 SPI_SendData8(SPI1,data);
		 data =  SPI_ReceiveData8(SPI1);
		 if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)) // for checking of tx buffer empty.
		 {
		 }
		 else if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))  // for checking of Rx buffer full.
		 {
		 }
		 */
	}
}
