#include <stdio.h>

#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "semphr.h"


#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

SemaphoreHandle_t lock;

#define HIGH_TASK_PRIORITY    ( tskIDLE_PRIORITY + 1UL )
#define MED_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define LOW_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void high_prio_task(__unused void *params) {
    printf("High priority task started.");
    vTaskDelay(1000);
    if(xSemaphoreTake(lock, 10000) == pdTRUE)
    {
        while(1) {}
        xSemaphoreGive(lock);
    }
    else
    {
        printf("HIGH: Request to take lock timed out");
    }
}

void med_prio_task(__unused void *params) {
    printf("Med priority task started.");
    vTaskDelay(500);
    if(xSemaphoreTake(lock, 10000) == pdTRUE)
    {
        while(1) {}
        xSemaphoreGive(lock);
    }
    else
    {
        printf("MED: Request to take lock timed out");
    }
}

void low_prio_task(__unused void *params) {
    printf("Low priority task started.");
    if(xSemaphoreTake(lock, 10000) == pdTRUE)
    {
        while(1) {}
        xSemaphoreGive(lock);
    }
    else
    {
        printf("LOW: Request to take lock timed out");
    }
}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";

    lock = xSemaphoreCreateBinary();
    TaskHandle_t task;
    xTaskCreate(high_prio_task, "HighPrioThread",
                MAIN_TASK_STACK_SIZE, NULL, HIGH_TASK_PRIORITY, &task);
    xTaskCreate(med_prio_task, "MedPrioThread",
                MAIN_TASK_STACK_SIZE, NULL, MED_TASK_PRIORITY, &task);
    xTaskCreate(low_prio_task, "LowPrioThread",
                MAIN_TASK_STACK_SIZE, NULL, LOW_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}