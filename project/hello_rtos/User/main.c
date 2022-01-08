#include "FreeRTOS.h"
#include "task.h"

#include "LED.h"

static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;


static void LED_Task(void) {
    while (1) {
        LED1_on();
        vTaskDelay(500);
        LED1_off();
        vTaskDelay(500);
    }
}

static void AppTaskCreate(void) {
    taskENTER_CRITICAL();
    
    xTaskCreate((TaskFunction_t)LED_Task, "LED_Task",
                 512, NULL, 2, &LED_Task_Handle);
    
    vTaskDelete(AppTaskCreate_Handle);
    
    taskEXIT_CRITICAL();
}

int main(void) {
    BaseType_t xRet = pdFAIL;
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    LED_init();
    
    xRet = xTaskCreate((TaskFunction_t)AppTaskCreate, "AppTaskCreate", 
                       512, NULL, 1, &AppTaskCreate_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
