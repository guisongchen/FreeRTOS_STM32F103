#include "FreeRTOS.h"
#include "task.h"

#include "Infrared_encode.h"
#include "OLED.h"

static TaskHandle_t OLED_Task_Handle = NULL;
static TaskHandle_t InfraCounter_Task_Handle = NULL;

static void OLED_Task(void *param) {
	OLED_ShowString(1, 1, "Encode cnt: ");
    while (1) {
		OLED_ShowNum(1, 13, InfraredEncode_getCnt(), 4);
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	OLED_Init();
	InfraredEncode_init();
	
	xRet = xTaskCreate((TaskFunction_t)OLED_Task, "OLED_Task", 
					   512, NULL, 1, &OLED_Task_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
