#include "LED.h"

void LED_init() {
    GPIO_InitTypeDef GPIOConf;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIOConf.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOConf.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIOConf);
    
    // turn off light
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED1_on() {
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
}

void LED1_off() {
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}

void LED1_revert() {
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	}
}

void LED2_on() {
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
}

void LED2_off() {
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
}

void LED2_revert() {
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
	}
}
