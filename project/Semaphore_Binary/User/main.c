#include "FreeRTOS.h"
#include "task.h"
//#include "queue.h"
#include "semphr.h"

#include "KEY.h"
#include "USART.h"


static TaskHandle_t Send_Task_Handle = NULL;
static TaskHandle_t Receive_Task_Handle = NULL;

SemaphoreHandle_t BinarySem_Handle = NULL;


static void Send_Task(void *param) {
	BaseType_t xRet = pdFAIL;
	uint8_t data = 0;
    while (1) {
		data = Key_getNum();
		if (data != 0) {
			xRet = xSemaphoreGive(BinarySem_Handle);
			
			if (xRet == pdPASS) {
				printf("Give Semaphore successed!\n");
			} else {
				printf("Give Semaphore failed! return val:%ld\n", xRet);
			}
		}
    }
	
	vTaskDelay(20);
}

static void Receive_Task(void *param) {
	BaseType_t xRet = pdFAIL;
	uint8_t data = 0;
    while (1) {
		xRet = xSemaphoreTake(BinarySem_Handle, portMAX_DELAY);
		if (xRet == pdPASS) {
			printf("Take Semaphore successed!\n");
		} else {
			printf("Take Semaphore failed!\n");
		}
    }
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
	
	BinarySem_Handle = xSemaphoreCreateBinary();
		
	xRet = xTaskCreate((TaskFunction_t)Send_Task, "Send_Task", 
					   512, NULL, 1, &Send_Task_Handle);
	
	if (xRet == pdPASS) {
		printf("Send task created!\n");
	} else {
		printf("Send task failed!\n");
		return -1;
	}
	
	xRet = xTaskCreate((TaskFunction_t)Receive_Task, "Receive_Task", 
					   512, NULL, 1, &Receive_Task_Handle);
    
    if (xRet == pdPASS) {
		printf("Receive_Task created!\n");
        vTaskStartScheduler();
    } else {
		printf("Receive_Task failed!\n");
        return -1;
    }
	
    
    while (1);
}
