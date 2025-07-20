#ifndef USB_RNDIS_TASK_H
#define USB_RNDIS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

// Task-Funktion für USB RNDIS (TinyUSB Device)
void usb_rndis_task(void *pvParameters);

// Hilfsfunktion zum Starten des USB RNDIS Tasks
void start_usb_rndis_task(void);

#endif // USB_RNDIS_TASK_H