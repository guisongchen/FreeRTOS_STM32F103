#include "USART.h"

// Disable semihosting mode
void _sys_exit(int x) {}
struct __FILE {
int handle;
};
FILE __stdout;
FILE __stdin;

// redirect stdclib printf to UART

int fputc(int ch, FILE *f) {
	USART_SendData(DEBUG_USART, (uint8_t) ch);
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	return (ch);
}

// redirect stdclib scanf to UART

int fgetc(FILE *f) {
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USART);
}


void USART_Init_User(void) {
	GPIO_InitTypeDef GPIOConf;
	USART_InitTypeDef USARTConf;
	NVIC_InitTypeDef NVICConf;
	
	// Enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(DEBUG_RCC_APB2Periph, ENABLE);
	
	// GPIO config
	// USART Tx
	GPIOConf.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIOConf.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIOConf);	
	// USART Rx
	GPIOConf.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_GPIO_PORT, &GPIOConf);
	
	// USART config

	USARTConf.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USARTConf.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTConf.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTConf.USART_Parity = USART_Parity_No;
	USARTConf.USART_StopBits = USART_StopBits_1;
	USARTConf.USART_WordLength = USART_WordLength_8b;
	USART_Init(DEBUG_USART, &USARTConf);
	
	// NVIC config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVICConf.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVICConf.NVIC_IRQChannelPreemptionPriority = 1;
	NVICConf.NVIC_IRQChannelSubPriority = 0;
	NVICConf.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICConf);
	
	// Enable USART receive interrupt
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
	// Notice: Not need to set USART_IT_TC (aka transmit enable)
	// we will use USART_GetFlagStatus(pUSART, USART_FLAG_TC) to read USART->SR TC bit
	// TC bit was set 1 by hardware, after sending 1 character each time 
	
	// Enable USART
	USART_Cmd(DEBUG_USART, ENABLE);
}

// sending process
// CPU --> Send Data Register --> Send Shift Register --> Tx

// Receiving process
// CPU <-- RECV Data Register <-- RECV Shift Register <-- Rx

void USART_SendByte_User(USART_TypeDef *pUSART, uint8_t ch) {
	USART_SendData(pUSART, ch);
	// USART_FLAG_TXE == RESET
	// data register was busy, not avaliable to update unfinish bits
	while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);
}

void USART_SendArray_User( USART_TypeDef * pUSART, uint8_t *array, uint16_t num) {
	uint8_t i;
	
	for (i = 0; i < num; i++) {
		USART_SendByte_User(pUSART, array[i]);
	}
	// USART_FLAG_TC == RESET
	// Shift register was busy, data sending was not finish
	while (USART_GetFlagStatus(pUSART, USART_FLAG_TC) == RESET);	
}

void USART_SendString_User( USART_TypeDef * pUSART, char *str) {
	uint32_t k = 0;
	while (str[k] != '\0'){
		USART_SendByte_User(pUSART, str[k]);
		k++;
	}
	
	// send '\0'
	USART_SendByte_User(pUSART, str[k]);
	while (USART_GetFlagStatus(pUSART, USART_FLAG_TC) == RESET);
}

void USART_SendHalfWord_User( USART_TypeDef * pUSART, uint16_t ch) {
	uint8_t temp_h, temp_l;
	
	temp_h = (ch & 0XFF00) >> 8;
	temp_l = ch & 0XFF;
	
	USART_SendData(pUSART,temp_h);	
	while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);
	
	USART_SendData(pUSART,temp_l);	
	while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);	
}