#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "KEY.h"
#include "USART.h"


static TaskHandle_t LED_Task_Handle = NULL;
static TaskHandle_t KEY_Task_Handle = NULL;

static void KEY_Task(void *param) {
	printf("Create KEY_Task!\n");
	BaseType_t xRet = pdFAIL;
	uint8_t data = 0;
    while (1) {
		data = Key_getNum();
		if (data != 0) {
			printf("KEY %d pressed.\n", data);
			xTaskNotify(LED_Task_Handle, 0x01 << (data-1), eSetBits);
		}
		vTaskDelay(20);
    }
}

static void LED_Task(void *param) {
	printf("Create LED_Task!\n");
	uint32_t lastNotify = 0;
	uint32_t currNotify = 0;
	BaseType_t xRet = pdFAIL;
    while (1) {
		xRet = xTaskNotifyWait(0x0, 0xFFFFFFFF, &currNotify, portMAX_DELAY);
		
		if (xRet == pdTRUE) {
//			printf("last:%x, curr:%x\n", lastNotify, currNotify);
			currNotify |= lastNotify;
			if (currNotify == (0x01 | 0x02)) {
				lastNotify = 0x0;
				printf("BOTH KEY PRESSED\n");
			} else {
				lastNotify = currNotify;
			}
		}
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
	
	xRet = xTaskCreate((TaskFunction_t)KEY_Task, "KEY_Task", 
					   512, NULL, 3, &KEY_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create KEY_Task failed!\n");
		return -1;
	}
	
	xRet = xTaskCreate((TaskFunction_t)LED_Task, "LED_Task", 
					   512, NULL, 2, &LED_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create LED_Task failed!\n");
		return -1;
	}
	

	
	vTaskStartScheduler();
	
    
    while (1);
}
