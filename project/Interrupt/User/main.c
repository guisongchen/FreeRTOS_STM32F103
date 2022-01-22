#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "KEY.h"
#include "USART.h"
#include "KEY_interrupt.h"

//#include <string.h>


static TaskHandle_t LED_Task_Handle = NULL;
static TaskHandle_t Receive_Task_Handle = NULL;

QueueHandle_t que = NULL;

static void LED_Task(void *param) {
	printf("Create LED_Task!\n");
	BaseType_t xRet = pdFAIL;
	uint32_t data;
    while (1) {
		xRet = xQueueReceive(que, &data, portMAX_DELAY);
		
		if (xRet == pdPASS)
			printf("KEY %d interrupt!\n", data);
		else
			printf("Queue receive failed!\n");
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	KEY_Interrupt_Init();
	USART_Init_User();
	
	que = xQueueCreate(4, 4);
	
	xRet = xTaskCreate((TaskFunction_t)LED_Task, "LED_Task", 
					   512, NULL, 2, &LED_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create LED_Task failed!\n");
		return -1;
	}
	
	vTaskStartScheduler();
	
    
    while (1);
}
