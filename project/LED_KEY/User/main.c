#include "FreeRTOS.h"
#include "task.h"

#include "LED.h"
#include "KEY.h"

//static TaskHandle_t LED1_Task_Handle = NULL;
//static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t KEY_Task_Handle = NULL;


//static void LED1_Task(void *param) {
//    while (1) {
//        LED1_on();
//        vTaskDelay(500);
//        LED1_off();
//        vTaskDelay(500);
//    }
//}

//static void LED2_Task(void *param) {
//    while (1) {
//        LED2_on();
//        vTaskDelay(500);
//        LED2_off();
//        vTaskDelay(500);
//    }
//}

static void KEY_Task(void *param) {
    while (1) {
		if (Key_getNum() == 1) {
			LED1_revert();
		} else if (Key_getNum() == 2) {
			LED2_revert();
		}
    }
}

int main(void) {
    BaseType_t xRet = pdFAIL;
    LED_init();
	Key_init();	
    
//    xRet = xTaskCreate((TaskFunction_t)LED1_Task, "LED1_Task", 
//                       512, NULL, 1, &LED1_Task_Handle);
//	
//	if (xRet == pdFAIL)
//		return -1;
//	
//    xRet = xTaskCreate((TaskFunction_t)LED2_Task, "LED2_Task", 
//                       512, NULL, 1, &LED2_Task_Handle);
//	
//	if (xRet == pdFAIL)
//		return -1;
	
	xRet = xTaskCreate((TaskFunction_t)KEY_Task, "KEY_Task", 
					   512, NULL, 1, &KEY_Task_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
