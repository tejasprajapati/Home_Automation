#include "stm32f0xx_rcc.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_gpio.h"
void spi_init(void);
int main(void) {

	spi_init();
	setup();
	while (1)
	{

	//	loop_T(); not needed for receiver at this place.
		loop_R();
	}
}
void spi_init(void)
	{
	    GPIO_InitTypeDef GPIO_InitStructure;
	    SPI_InitTypeDef  SPI_InitStructure;

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
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOC, &GPIO_InitStructure);

	    /* Deselect : Chip Select high */
	    GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
