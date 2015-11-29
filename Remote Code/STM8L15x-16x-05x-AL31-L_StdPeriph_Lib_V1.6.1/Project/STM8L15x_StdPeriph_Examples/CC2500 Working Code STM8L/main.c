/**
  ******************************************************************************
  * @file     SPI_FullDuplexUART1\main.c
  * @author   MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
  * @brief    This file contains the main function for SPI full duplex 
  *          communication with UART1 example.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "rad.h"

/**
  * @addtogroup SPI_FullDuplexUART1
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

uint8_t TxBuffer1[] = "UART1 Example: UART1 -> SPI using TXE and RXNE Flags";
uint8_t TxBuffer2[] = "UART1 Example: SPI -> UART1 using TXE and RXNE Flags";
//uint8_t RxBuffer1[TxBufferSize2] = {0};
uint8_t RxBuffer2[TxBufferSize1] = {0};
uint8_t NbrOfDataToRead = TxBufferSize1;
unsigned char y;
//__IO uint8_t TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Delay (uint16_t nCount);
/* Private functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /*High speed internal clock prescaler: 1*/
   //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);

  /* SPI configuration */
  SPI_DeInit(SPI1);
  /* Initialize SPI in Slave mode  */
  SPI_Init(SPI1,SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master, SPI_CPOL_Low,
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft,(uint8_t)0x07);

  GPIO_Init(GPIOD,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
  /* Enable the UART1*/
  Delay(0xFFF);
  
  /* Enable the SPI*/
  SPI_Cmd(SPI1,ENABLE);
  setup();
  while(1)
  {
//    WriteReg(0x00,0x29);
//    y=Read(0x80);
//    ss_low;    //lo
//
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
//    SPI_SendData(SPI1, 0x00); // putcSPI1(add);         			//SEND ADR BYTE
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    y = SPI_ReceiveData(SPI1);
//
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
//    SPI_SendData(SPI1, 0x29); //SEND PROXY DATA
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    y = SPI_ReceiveData(SPI1); //getcSPI1();
//    
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
//    SPI_SendData(SPI1, 0x80); // putcSPI1(add);         			//SEND ADR BYTE
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    y = SPI_ReceiveData(SPI1);
//
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET);
//    SPI_SendData(SPI1, 0x00); //SEND PROXY DATA
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    y = SPI_ReceiveData(SPI1); //getcSPI1();
//    
//    ss_high;
}
//    while (1)
//    {
//        /* Wait until end of transmit */
//        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)== RESET)
//        {
//        }
//        /* Write one byte in the SPI Transmit Data Register */
//        SPI_SendData(SPI1,TxBuffer2[TxCounter++]);
////        Delay(0xFFFF);
////        Delay(0xFFFF);
//        /* Write one byte in the UART1 Transmit Data Register */
////        UART1_SendData8(TxBuffer1[TxCounter++]);
//        /* Wait until end of transmit */
////        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
////        {
////        }
////        /* Wait the byte is entirely received by UART1 */
////        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
////        {
////        }
////        /* Store the received byte in the RxBuffer1 */
////        RxBuffer1[RxCounter] = UART1_ReceiveData8();
////        /* Wait the byte is entirely received by SPI */
//        while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET)
//        {
//        }
//        /* Store the received byte in the RxBuffer2 */
//        RxBuffer2[RxCounter++] = SPI_ReceiveData(SPI1);
//        Delay(0xFFFF);
//        Delay(0xFFFF);
//        if(RxCounter==0x34)
//        {
//          RxCounter=0;
//          while(1);
//        }
//    }

    /* Check the received data with the sent ones */
//    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and received by SPI are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and received by SPI are different */
//    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from SPI and received by UART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from SPI and received by UART11 are different */

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
