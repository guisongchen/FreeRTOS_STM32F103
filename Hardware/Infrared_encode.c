#include "Infrared_encode.h"

uint16_t cnt;

void InfraredEncode_init(void) {
    GPIO_InitTypeDef GPIOConf;
    EXTI_InitTypeDef EXTIConf;
    
    // GPIO config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIOConf.GPIO_Mode = GPIO_Mode_IPU;
    GPIOConf.GPIO_Pin = GPIO_Pin_14;
    GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIOConf);
    
    // EXTI config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    
    EXTIConf.EXTI_Line = EXTI_Line14;
    EXTIConf.EXTI_LineCmd = ENABLE;
    EXTIConf.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTIConf.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTIConf);
    
    // NVIC config
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVICConf;
    NVICConf.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVICConf.NVIC_IRQChannelCmd = ENABLE;
    NVICConf.NVIC_IRQChannelPreemptionPriority = 1;
    NVICConf.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVICConf);
    
    cnt = 0;
}

uint16_t InfraredEncode_getCnt(void) {
    return cnt;
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line14) == SET) {
        cnt++;
        
        // must clear interrupt bit, otherwise it will stuck here
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}
