#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* RP2040: Verwende Pico SDK ISR Handler */
#define vPortSVCHandler        isr_svcall
#define xPortPendSVHandler     isr_pendsv
#define xPortSysTickHandler    isr_systick

/* Basis-Konfiguration */
#define configUSE_PREEMPTION                    1
#define configCPU_CLOCK_HZ                      133000000UL
#define configTICK_RATE_HZ                      1000          /* 1ms Tick */
#define configMAX_PRIORITIES                    7
#define configMINIMAL_STACK_SIZE                256           /* Empfohlen für RP2040 */
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TIME_SLICING                  1

/* Speicherverwaltung */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        1

/* Benachrichtigungen & Synchronisation */
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               10

/* Software-Timer */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Hook-Funktionen */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Statistiken & Tracing */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-Routinen */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Assert */
#define configASSERT( x )                       if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Optional: API-Funktionen */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xTaskResumeFromISR              1

#endif /* FREERTOS_CONFIG_H */