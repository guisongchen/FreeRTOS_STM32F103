#include "FreeRTOS.h"
#include "task.h"

#include "USART.h"
#include "OLED.h"

static TaskHandle_t USART_Task_Handle = NULL;

static void USART_Task(void *param) {
	printf("LED_Task!\n");
	printf("LED_Task!\n");
	OLED_ShowNum(1, 1, 666, 3);
    while (1) {
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	OLED_Init();
	USART_Init_User();
	
	xRet = xTaskCreate((TaskFunction_t)USART_Task, "USART_Task", 
					   512, NULL, 1, &USART_Task_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
