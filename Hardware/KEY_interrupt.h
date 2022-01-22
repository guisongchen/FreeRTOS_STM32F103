#ifndef __KEY_INTERRUPT_H
#define __KEY_INTERRUPT_H

#include "stm32f10x.h"                  // Device header

#define KEY_RCC_APB2Periph 	RCC_APB2Periph_GPIOB
#define KEY_GPIO_PORT		GPIOB

#define KEY1_GPIO_PIN		GPIO_Pin_1
#define KEY2_GPIO_PIN		GPIO_Pin_11

#define KEY1_INT_EXTI_IRQ	EXTI1_IRQn
#define KEY2_INT_EXTI_IRQ	EXTI15_10_IRQn

#define KEY1_IRQHandler		EXTI1_IRQHandler
#define KEY2_IRQHandler		EXTI15_10_IRQHandler

#define KEY1_INT_EXTI_LINE	EXTI_Line1
#define KEY2_INT_EXTI_LINE	EXTI_Line11

#define KEY1_INT_EXTI_PORTSOURCE	GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PORTSOURCE	GPIO_PortSourceGPIOB

#define KEY1_INT_EXTI_PINSOURCE	GPIO_PinSource1
#define KEY2_INT_EXTI_PINSOURCE	GPIO_PinSource11

void KEY_Interrupt_Init();

#endif // __KEY_INTERRUPT_H
