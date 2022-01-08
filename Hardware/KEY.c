#include "KEY.h"

void Key_init() {
    GPIO_InitTypeDef GPIOConf;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIOConf.GPIO_Mode = GPIO_Mode_IPU;
    GPIOConf.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIOConf);
}

uint8_t Key_getNum() {
    uint8_t num = 0;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
        Delay_ms(20);
        num = 1;
    }
    
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
        Delay_ms(20);
        num = 2;
    }
    
    return num;
}
