#include "RotationEncode.h"

int16_t RotEncode_counter;

void RotationEncode_init(void) {
    GPIO_InitTypeDef GPIOConf;
    EXTI_InitTypeDef EXTIConf;
    
    // GPIO config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIOConf.GPIO_Mode = GPIO_Mode_IPU;
    GPIOConf.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIOConf.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIOConf);
    
    // EXTI config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    
    EXTIConf.EXTI_Line = EXTI_Line0 | EXTI_Line1;
    EXTIConf.EXTI_LineCmd = ENABLE;
    EXTIConf.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTIConf.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTIConf);
    
    // NVIC config
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitTypeDef NVICConf;
    NVICConf.NVIC_IRQChannel = EXTI0_IRQn; // Pin 0: A
    NVICConf.NVIC_IRQChannelCmd = ENABLE;
    NVICConf.NVIC_IRQChannelPreemptionPriority = 1;
    NVICConf.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVICConf);
    
    NVIC_InitTypeDef NVICConfSecond;
    NVICConfSecond.NVIC_IRQChannel = EXTI1_IRQn; // Pin 1: B
    NVICConfSecond.NVIC_IRQChannelCmd = ENABLE;
    NVICConfSecond.NVIC_IRQChannelPreemptionPriority = 2;
    NVICConfSecond.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVICConfSecond);
    
    RotEncode_counter = 0;
}

// change since last call
int16_t RotationEncode_getChangeCnt(void) {
    int16_t ret = RotEncode_counter;
    RotEncode_counter = 0;
    return ret;
}


// EXTI0 handler
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        // check pin 1 status to decide direction
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
            RotEncode_counter--;  // counterclockwise
        }       
        
        // must clear interrupt bit, otherwise it will stuck here
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

// EXTI1 handler
void EXTI1_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line1) == SET) {
        // check pin 1 status to decide direction
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
            RotEncode_counter++;  // clockwise
        }       
        
        // must clear interrupt bit, otherwise it will stuck here
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
