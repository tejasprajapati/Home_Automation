#include "stm32f0xx_rcc.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"
#include "global.h"
#include "stm32f0xx_pwr.h"
#include "stm32f0xx_misc.h"
//#include "rad.h"

//#define Tx_mode
#define Rx_mode

void spi_init(void);
int main(void)
{
	spi_init();
	setup();

	#ifdef Rx_mode
		cc2500_mode(1);  //configure device in rx/tx mode (1 - rx ,0 - tx)
	#else if Tx_mode
		cc2500_mode(0);
	#endif
	while (1)
	{
		request_received = 1;  // for testing only.
//		PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);  //wfi();// commented temporary by rb must remove it.(15/11/14)
		if (request_received)   // must be high on external interrupt
		{
			request_received = 0;
			#ifdef Rx_mode
				RxData();
			#else if Tx_mode
				TxData("id+key no which is pressed");
			#endif
		}
	//	loop_T(); not needed for receiver at this place.
	//	loop_R();
		if(take_action!=0)
		{
			switch(take_action)
			{
				case 49:
					led_toggle;
				case 50:
					;
				default:
					;
			}
			take_action = 0;
		}
	}
}

void spi_init(void)
	{
	    GPIO_InitTypeDef GPIO_InitStructure;
	    SPI_InitTypeDef  SPI_InitStructure;
	    EXTI_InitTypeDef   EXTI_InitStructure;
	    NVIC_InitTypeDef   NVIC_InitStructure;

	    /* Enable the SPI periph */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//RCC_APB2Periph_SPI1

	    /* Enable SCK, MOSI and MISO GPIO clocks */
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);

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
	    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	    /* SPI SCK pin configuration */
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    /* SPI  MOSI pin configuration */
	    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    /* SPI MISO pin configuration */
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    /* SPI configuration -------------------------------------------------------*/
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

	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	    /* Deselect : Chip Select high */

//	    /* Configure GPIO PIN for GDO status *///need to configure as an interrupt 28/9/14
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	   	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	   	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	   	  /* Connect EXTI0 Line to PA0 pin */
	   	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

	   	  /* Configure EXTI0 line */
	   	  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	   	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	   	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	   	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	   	  EXTI_Init(&EXTI_InitStructure);

	   	  /* Enable and set EXTI0 Interrupt */
	   	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	   	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	   	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	   	  NVIC_Init(&NVIC_InitStructure);

//	   	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//	   	    EXTI_InitTypeDef EXTI_InitStruct;
//	   	    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0); // PA0 configured as an interrupt for testing
//	   	    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
//	   	    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	   		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
//	   		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//			EXTI_StructInit(&EXTI_InitStruct);


//			NVIC_InitTypeDef   NVIC_InitStructure;
//			NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
//			NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
//			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//			NVIC_Init(&NVIC_InitStructure);

//	   	    /* Deselect : Chip Select high */
//	   	    GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}

void EXTI0_1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
	/* Toggle LED3 and LED4 */
//			    GPIOC->ODR ^= GPIO_Pin_9;
//			    GPIOC->ODR ^= GPIO_Pin_8;

	/* Clear the EXTI line 0 pending bit */
	  request_received = 1;  // for testing only.
	EXTI_ClearITPendingBit(EXTI_Line1);
  }
}
