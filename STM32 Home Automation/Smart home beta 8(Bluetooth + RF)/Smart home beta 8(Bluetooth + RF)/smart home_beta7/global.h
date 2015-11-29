unsigned char request_received,take_action;
#define led_toggle GPIOC->ODR ^= GPIO_Pin_8;
#define led_toggle1 GPIOC->ODR ^= GPIO_Pin_9;
#define led_off GPIOC->ODR &= ~(GPIO_Pin_8 | GPIO_Pin_9);

#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_TXFIFO  0x3F		 // TX FIFO BUFFER
#define CC2500_RXFIFO  0x3F      // RX FIFO BUFFER

void WriteReg(char add,char data);
char ReadReg(char add);
char Read(char add);
char SendStrobe(char strobe);
void init_CC2500(void);
void Read_Config_Regs(void);
void setup(void);
void cc2500_mode(char);
void TxData(unsigned char*);
void RxData(void);
void interrupt_enable(void);

// bluetooth
void usart_rxtx(void);
//void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
