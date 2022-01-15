#include "FreeRTOS.h"
#include "task.h"

#include "RotationEncode.h"
#include "OLED.h"

static TaskHandle_t OLED_Task_Handle = NULL;
int16_t num;

static void OLED_Task(void *param) {
	OLED_ShowString(1, 1, "Encode num: ");
    while (1) {
		num += RotationEncode_getChangeCnt();
        OLED_ShowSignedNum(1, 12, num, 4);
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	OLED_Init();
	RotationEncode_init();	
	xRet = xTaskCreate((TaskFunction_t)OLED_Task, "OLED_Task", 
					   512, NULL, 1, &OLED_Task_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
