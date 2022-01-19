#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "KEY.h"
#include "USART.h"


static TaskHandle_t HighPriority_Task_Handle = NULL;
static TaskHandle_t MidPriority_Task_Handle = NULL;
static TaskHandle_t LowPriority_Task_Handle = NULL;

SemaphoreHandle_t BinarySem_Handle = NULL;

static void LowPriority_Task(void *param) {
	printf("Create LowPriority_Task!\n");
	BaseType_t xRet = pdFAIL;
	uint32_t i = 0;
    while (1) {
		printf("Low priority task trying to take semaphore\n");
		xRet = xSemaphoreTake(BinarySem_Handle, portMAX_DELAY);

		
		if (xRet == pdPASS) {
			printf("Low priority task running\n");
		}
		
		for (i = 0; i < 2000000; i++) {
			taskYIELD();
		}
		
		xRet = xSemaphoreGive(BinarySem_Handle);
		printf("Low priority task release semaphore\n");
		vTaskDelay(500);
    }
}

static void MidPriority_Task(void *param) {
	printf("Create MidPriority_Task!\n");
	while (1) {
		printf("Mid priority task running\n");
		vTaskDelay(500);
	}
}

static void HighPriority_Task(void *param) {
	printf("Create HighPriority_Task!\n");
	BaseType_t xRet = pdFAIL;
    while (1) {
		printf("High priority task trying to take semaphore\n");
		xRet = xSemaphoreTake(BinarySem_Handle, portMAX_DELAY);
		
		if (xRet == pdPASS) {
			printf("High priority task running\n");
		}
		
		xRet = xSemaphoreGive(BinarySem_Handle);
		printf("High priority task release semaphore\n");
		
		vTaskDelay(500);
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
	
	BinarySem_Handle = xSemaphoreCreateBinary();
	xSemaphoreGive(BinarySem_Handle);
	
	xRet = xTaskCreate((TaskFunction_t)LowPriority_Task, "LowPriority_Task", 
					   512, NULL, 3, &LowPriority_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create LowPriority_Task failed!\n");
		return -1;
	}
	
	xRet = xTaskCreate((TaskFunction_t)MidPriority_Task, "MidPriority_Task", 
					   512, NULL, 4, &MidPriority_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create MidPriority_Task failed!\n");
		return -1;
	}
	
	xRet = xTaskCreate((TaskFunction_t)HighPriority_Task, "HighPriority_Task", 
					   512, NULL, 5, &HighPriority_Task_Handle);
	
	if (xRet == pdFAIL) {
		printf("Create HighPriority_Task failed!\n");
		return -1;
	}
	

	
	vTaskStartScheduler();
	
    
    while (1);
}
