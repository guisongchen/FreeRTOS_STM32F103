#include "FreeRTOS.h"
#include "task.h"

#include "KEY.h"
#include "USART.h"

static TaskHandle_t KEY_Task_Handle = NULL;

uint8_t *dataPtr = NULL;

static void KEY_Task(void *param) {
	printf("Create KEY_Task!\n");
	BaseType_t xRet = pdFAIL;
	uint32_t memsize;
	uint8_t keyNum = 0;
    while (1) {
		keyNum = Key_getNum();
		if (keyNum == 1) {
			printf("KEY 1 pressed.\n");
			memsize = xPortGetFreeHeapSize();
			printf("curr memsize:%d, start allocate mem\n", memsize);
			if (dataPtr != NULL) {
				printf("last allocate mem not free, press key 2 to free\n");
				continue;
			}
			
			dataPtr = pvPortMalloc(1024);
			if (dataPtr != NULL) {
				printf("mem alloc successed! addr:%#X\n", (int)dataPtr);
				
				memsize = xPortGetFreeHeapSize();
				printf("remaining curr memsize:%d\n", memsize);
				
				sprintf((char*)dataPtr, "system time=%d\n", xTaskGetTickCount());
				printf("writen content:%s\n", (char*)dataPtr);
			} else {
				printf("mem empty, free mem by pressing KEY 2!\n");
			}
		} else if (keyNum == 2) {
			if (dataPtr != NULL) {
				printf("start free mem\n");
				vPortFree(dataPtr);
				dataPtr = NULL;
				memsize = xPortGetFreeHeapSize();
				printf("mem free done, curr memsize:%d\n", memsize);
			} else {
				printf("no mem was allocated!\n");
			}
		}
		vTaskDelay(20);
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

	
	vTaskStartScheduler();
	
    
    while (1);
}
