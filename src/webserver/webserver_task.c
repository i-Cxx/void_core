#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"

#include "tusb_lwip_glue.h"
#include "lwip/apps/httpd.h"
#include "webserver.h"

#define LED_PIN 25

// CGI-Handler: LED toggeln
static const char *cgi_toggle_led(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
    return "/index.html";
}

// CGI-Handler: USB Boot Reset
static const char *cgi_reset_usb_boot(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    reset_usb_boot(0, 0);
    return "/index.html";
}

static const tCGI cgi_handlers[] = {
    { "/toggle_led", cgi_toggle_led },
    { "/reset_usb_boot", cgi_reset_usb_boot }
};

void webserver_task(void *pvParameters) {
    // Netzwerk initialisieren
    init_lwip();
    wait_for_netif_is_up();
    dhcpd_init();

    // HTTP Server starten
    httpd_init();
    http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    for (;;) {
        service_traffic();  // LWIP Netzwerk-Handling
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void start_webserver_task(void) {
    xTaskCreate(webserver_task, "Webserver", 2048, NULL, tskIDLE_PRIORITY + 3, NULL);
}