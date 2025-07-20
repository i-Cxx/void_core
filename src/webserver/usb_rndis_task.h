#include "FreeRTOS.h"
#include "task.h"
#include "tusb.h"

void usb_rndis_task(void *pvParameters) {
    while (1) {
        tud_task();  // TinyUSB device task
        vTaskDelay(pdMS_TO_TICKS(1));  // möglichst oft aufrufen
    }
}

void start_usb_rndis_task(void) {
    xTaskCreate(usb_rndis_task, "USB RNDIS", 512, NULL, tskIDLE_PRIORITY+3, NULL);
}