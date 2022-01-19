#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "KEY.h"
#include "USART.h"


static TaskHandle_t LED_Task_Handle = NULL;
static TaskHandle_t MidPriority_Task_Handle = NULL;
static TaskHandle_t KEY_Task_Handle = NULL;

EventGroupHandle_t Event_Handle = NULL;

static void KEY_Task(void *param) {
	printf("Create KEY_Task!\n");
	BaseType_t xRet = pdFAIL;
	uint8_t data = 0;
    while (1) {
		data = Key_getNum();
		if (data != 0) {
			printf("KEY %d pressed.\n", data);
			xEventGroupSetBits(Event_Handle, 0x01 << (data-1));
		}
		vTaskDelay(20);
    }
}

static void LED_Task(void *param) {
	printf("Create LED_Task!\n");
	EventBits_t event;
    while (1) {
		event = xEventGroupWaitBits(Event_Handle, 0x01 | 0x02, 
									pdTRUE, pdTRUE, portMAX_DELAY);
		
		if ((event & (0x01 | 0x02)) == (0x01 | 0x02))
			printf("BOTH KEY PRESSED\n");
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
	
	Event_Handle = xEventGroupCreate();
	
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
