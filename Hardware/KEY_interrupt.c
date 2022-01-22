#include "KEY_interrupt.h"

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t que;

static uint32_t data1 = 1;
static uint32_t data2 = 2;

void KEY_Interrupt_Init() {
	GPIO_InitTypeDef GPIOConf;
	NVIC_InitTypeDef NVICConf;
	EXTI_InitTypeDef EXTIConf;
	
	// clock
	RCC_APB2PeriphClockCmd(KEY_RCC_APB2Periph, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// GPIO config
	GPIOConf.GPIO_Mode = GPIO_Mode_IPU;
	GPIOConf.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN;
	GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_GPIO_PORT, &GPIOConf);
	
	// NVIC config
	// KEY 1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVICConf.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
	NVICConf.NVIC_IRQChannelCmd = ENABLE;
	NVICConf.NVIC_IRQChannelPreemptionPriority = 4;
	NVICConf.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVICConf);
	
	// KEY 2
	NVICConf.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
	NVIC_Init(&NVICConf);
	
	// EXTI config
	// KEY 1
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
	EXTIConf.EXTI_Line = KEY1_INT_EXTI_LINE;
	EXTIConf.EXTI_LineCmd = ENABLE;
	EXTIConf.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIConf.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTIConf);
	
	// KEY 2
	GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
	EXTIConf.EXTI_Line = KEY2_INT_EXTI_LINE;
	EXTIConf.EXTI_LineCmd = ENABLE;
	EXTIConf.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIConf.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTIConf);
}

void KEY1_IRQHandler() {
	BaseType_t wakeHighPriorTask = pdFALSE;
	if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
		xQueueSendFromISR(que, &data1, &wakeHighPriorTask);
		portYIELD_FROM_ISR(wakeHighPriorTask);
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	}
}

void KEY2_IRQHandler() {
	BaseType_t wakeHighPriorTask = pdFALSE;
	if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) {
		xQueueSendFromISR(que, &data2, &wakeHighPriorTask);
		portYIELD_FROM_ISR(wakeHighPriorTask);
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
	}
}

uint32_t KEY_GetNum() {
	return data1;
}