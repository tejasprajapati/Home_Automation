#include "stm8l15x.h"
#include "rad.h"
#include "global.h"
#include "stm8l15x_it.h"
void Delay (uint16_t nCount);

unsigned char *tx_id = "tx-123456";
unsigned char *rx_id = "rx-123456";
unsigned char test_str[14];
void gpio_init(void);
void spi_init(void);
void main(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  gpio_init();
  spi_init();
  setup();              // configure the cc2500 in required format.
  #ifdef Rx_mode
      cc2500_mode(1);  //configure device in rx/tx mode (1 - rx ,0 - tx)
  #else if Tx_mode
      cc2500_mode(0);
  #endif
  while (1)
  {
    wfi();                      //sleep mode
    key_pressed = 1;
    if(key_pressed)             // must be high on external interrupt
    {
        led(1);
        key_pressed = 0;
        generate_id(key_no);
        key_pressed = 0;
        #ifdef Rx_mode
                RxData();
        #else if Tx_mode
               strcpy(test_str,tx_id);
               TxData(tx_id);
//               Delay(500);
               led(0);
        #endif
    }
  }
}

INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);
  key_no = '0';
}
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
  key_no = '1';
}
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
  key_no = '2';
}
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
  key_no = '3';
}
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);
  key_no = '4';
}
void gpio_init(void)
{
  GPIO_Init(GPIOD,GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOC,GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOA,GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOF,GPIO_Pin_0, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOC,GPIO_Pin_1, GPIO_Mode_In_PU_IT);//  GPIO_Init(GPIOB,GPIO_Pin_1, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOB,GPIO_Pin_2, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOB,GPIO_Pin_3, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOB,GPIO_Pin_4, GPIO_Mode_In_PU_IT);
  
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Falling);
}
void spi_init(void)
{
  SPI_DeInit(SPI1);
  SPI_Init(SPI1,SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master, SPI_CPOL_Low,
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft,(uint8_t)0x07);
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
  Delay(0xFFF);
  SPI_Cmd(SPI1,ENABLE);
}
void generate_id(int no)                //for id generation
{
  char key_no_array[3];
  sprintf(key_no_array,"%d",no);
  strcat(tx_id,key_no_array);
}
/**
  * @brief   Compares two buffers.
  * @param   pBuffer1 First buffer to be compared.
  * @param   pBuffer2 Second buffer to be compared.
  * @param   BufferLength Buffer's length
  * @retval  TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
  */
//TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//    while (BufferLength--)
//    {
//        if (*pBuffer1 != *pBuffer2)
//        {
//            return FAILED;
//        }
//
//        pBuffer1++;
//        pBuffer2++;
//    }
//
//    return PASSED;
//}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
