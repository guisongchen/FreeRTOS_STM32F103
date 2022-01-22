#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "KEY.h"
#include "USART.h"

static TaskHandle_t Timer_Task_Handle = NULL;
static TimerHandle_t SW_Timer1_Handle = NULL;
static TimerHandle_t SW_Timer2_Handle = NULL;

static uint32_t cb1_cnt = 0;
static uint32_t cb2_cnt = 0;

static void Timer1_Callback(void *param) {
	TickType_t cnt = xTaskGetTickCount();
	cb1_cnt++;
	printf("Auto load: callback_cnt:%d,  tick_cnt:%d\n", cb1_cnt, cnt);
}

static void Timer2_Callback(void *param) {
	TickType_t cnt = xTaskGetTickCount();
	cb2_cnt++;
	printf("OneShot: callback_cnt:%d,  tick_cnt:%d\n", cb2_cnt, cnt);
}

static void Timer_Task(void *param) {
	printf("Create Timer_Task successed!\n");
	SW_Timer1_Handle = xTimerCreate("Timer1_Callback", (TickType_t)1000,
					   pdTRUE, (void*)1, (TimerCallbackFunction_t)Timer1_Callback);
	
	if (SW_Timer1_Handle == NULL) {
		printf("Create SW_Timer1_Handle failed!\n");
	} else {
		xTimerStart(SW_Timer1_Handle, 0);
	}
	
	SW_Timer2_Handle = xTimerCreate("Timer2_Callback", (TickType_t)5000,
					   pdFALSE, (void*)2, (TimerCallbackFunction_t)Timer2_Callback);
	
	if (SW_Timer2_Handle == NULL) {
		printf("Create SW_Timer2_Handle failed!\n");
		return;
	} else {
		xTimerStart(SW_Timer2_Handle, 0);
	}
	
	while (1);
}


int main(void) {
    BaseType_t xRet = pdFAIL;
	
	Key_init();
	USART_Init_User();
	
	xRet = xTaskCreate((TaskFunction_t)Timer_Task, "Timer_Task", 
						512, NULL, 1, (TaskHandle_t*)&Timer_Task_Handle);
	

	if (xRet == pdFAIL) {
		printf("Create Timer_Task failed!\n");
		return -1;
	}

	
	vTaskStartScheduler();
	
    
    while (1);
}
