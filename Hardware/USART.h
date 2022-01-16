#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>

// Disable semihosting mode
#pragma import(__use_no_semihosting) // enable use no semihosting function

#define DEBUG_USART 				USART1
#define DEBUG_RCC_APB2Periph 		RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE		115200

#define DEBUG_USART_GPIO_PORT		GPIOA
#define DEBUG_USART_TX_GPIO_PIN		GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PIN		GPIO_Pin_10

#define  DEBUG_USART_IRQ            USART1_IRQn

// use surffix _User distinguish lib function and user function
void USART_Init_User(void);
void USART_SendByte_User(USART_TypeDef *pUSART, uint8_t ch);
void USART_SendArray_User( USART_TypeDef * pUSART, uint8_t *array, uint16_t num);
void USART_SendString_User( USART_TypeDef * pUSART, char *str);
void USART_SendHalfWord_User( USART_TypeDef * pUSART, uint16_t ch);

#endif /* __USART_H */
