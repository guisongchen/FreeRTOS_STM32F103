#include "FreeRTOS.h"
#include "task.h"

#include "KEY.h"
#include "USART.h"


static TaskHandle_t Send_Task_Handle = NULL;
static TaskHandle_t Receive_Task_Handle = NULL;


static void Send_Task(void *param) {
	BaseType_t xRet = pdFAIL;
	uint8_t data = 0;
    while (1) {
		data = Key_getNum();
		if (data == 1) {
			xRet = xTaskNotifyGive(Receive_Task_Handle);
			printf("Give one.\n");
		}

    }
	
	vTaskDelay(20);
}

static void Receive_Task(void *param) {
	uint32_t xRet = 0;
	uint8_t data = 0;
    while (1) {
		data = Key_getNum();
		if (data == 2) {
			xRet = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
			printf("Take one, remianing cnt=%d!\n", --xRet);
			if (xRet == 0) {
				printf("Receive Task will be Blocked\n");
			}
		}
    }
	
	vTaskDelay(20);
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
		
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
