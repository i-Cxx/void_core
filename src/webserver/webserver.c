#include "webserver.h"

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"

#include "tusb_lwip_glue.h"

#define LED_PIN     25

// CGI Handler für LED toggle
static const char *cgi_toggle_led(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
    return "/index.html";
}

// CGI Handler für USB Reset Boot
static const char *cgi_reset_usb_boot(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    reset_usb_boot(0, 0);
    return "/index.html";
}

static const tCGI cgi_handlers[] = {
    { "/toggle_led", cgi_toggle_led },
    { "/reset_usb_boot", cgi_reset_usb_boot }
};

void vWebserverTask(void *pvParameters) {
    // Initialize networking stack
    init_lwip();
    wait_for_netif_is_up();
    dhcpd_init();

    // Initialize HTTP server
    httpd_init();
    http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        tud_task();
        service_traffic();
        vTaskDelay(pdMS_TO_TICKS(10));  // Wichtig, um CPU nicht voll zu belegen
    }
}

void start_webserver_task() {
    xTaskCreate(vWebserverTask, "WebserverTask", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
}