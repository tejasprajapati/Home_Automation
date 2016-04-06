#ifndef PIN_CONF.h
#define PIN_CONF.h

#ifdef DEV_KIT
#define led_toggle GPIOC->ODR ^= GPIO_Pin_8;
#define led_toggle1 GPIOC->ODR ^= GPIO_Pin_9;
#define led_off GPIOC->ODR &= ~(GPIO_Pin_8 | GPIO_Pin_9);
#endif /* DEV_KIT*/

#define DEV_1	GPIO_Pin_8;
#define DEV_2	GPIO_Pin_9;
#define DEV_3	GPIO_Pin_10;
#define DEV_4	GPIO_Pin_11;
#define DEV_5	GPIO_Pin_12;

#define DEV_1_ON GPIOC->ODR |= DEV_1;										/*	Device 1 is connected on this pin turn it on*/
#define DEV_2_ON GPIOC->ODR |= DEV_2;										/*	Device 2 is connected on this pin turn it on*/
#define DEV_3_ON GPIOC->ODR |= DEV_3;										/*	Device 3 is connected on this pin turn it on*/
#define DEV_4_ON GPIOC->ODR |= DEV_4;										/*	Device 4 is connected on this pin turn it on*/
#define DEV_5_ON GPIOC->ODR |= DEV_5;										/*	Device 5 is connected on this pin turn it on*/

#define DEV_1_OFF GPIOC->ODR &= ~DEV_1;										/*	Device 1 is connected on this pin turn it off*/
#define DEV_2_OFF GPIOC->ODR &= ~DEV_2;										/*	Device 2 is connected on this pin turn it off*/
#define DEV_3_OFF GPIOC->ODR &= ~DEV_3;										/*	Device 3 is connected on this pin turn it off*/
#define DEV_4_OFF GPIOC->ODR &= ~DEV_4;										/*	Device 4 is connected on this pin turn it off*/
#define DEV_5_OFF GPIOC->ODR &= ~DEV_5;										/*	Device 5 is connected on this pin turn it off*/

#define DEV_1_TOGGLE GPIOC->ODR ^= DEV_1;									/*	Device 1 is connected on this pin toggle it*/
#define DEV_2_TOGGLE GPIOC->ODR ^= DEV_2;									/*	Device 2 is connected on this pin toggle it*/
#define DEV_3_TOGGLE GPIOC->ODR ^= DEV_3;									/*	Device 3 is connected on this pin toggle it*/
#define DEV_4_TOGGLE GPIOC->ODR ^= DEV_4;									/*	Device 4 is connected on this pin toggle it*/
#define DEV_5_TOGGLE GPIOC->ODR ^= DEV_5;									/*	Device 5 is connected on this pin toggle it*/

#define ALL_OFF GPIOC->ODR &= ~(DEV_1 | DEV_2 |\
								DEV_3 | DEV_4 |\
								DEV_5);										/*	Turn off all the device all together*/

#endif
