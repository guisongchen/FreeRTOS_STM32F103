#include "FreeRTOS.h"
#include "task.h"

#include "LED.h"

static TaskHandle_t LED_Task_Handle = NULL;


static void LED_Task(void) {
    while (1) {
        LED1_on();
        vTaskDelay(1500);
        LED1_off();
        vTaskDelay(1500);
    }
}

int main(void) {
    BaseType_t xRet = pdFAIL;
    LED_init();
    
    xRet = xTaskCreate((TaskFunction_t)LED_Task, "LED_Task", 
                       512, NULL, 1, &LED_Task_Handle);
    
    if (xRet == pdPASS) {
        vTaskStartScheduler();
    } else {
        return -1;
    }
    
    while (1);
}
