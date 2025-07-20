#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "lcd_1602_i2c.h"
#include "ssd1306_i2c.h"
#include "webserver.h"         // Webserver-Task
#include "usb_rndis_task.h"    // USB-RNDIS-Task

#ifdef __cplusplus
extern "C" {
#endif
extern void vBlinkTaskCpp(void *pvParameters);
#ifdef __cplusplus
}
#endif

#define MY_CUSTOM_LED_PIN 25
#define WARN_LED 16

#define BLINK_TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define LCD1602_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)
#define SSD1306_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)
#define RNDIS_TASK_PRIORITY     (tskIDLE_PRIORITY + 4)    // Falls Priorität gebraucht wird
#define WEBSERVER_TASK_PRIORITY (tskIDLE_PRIORITY + 5)    // Falls Priorität gebraucht wird

#define LCD1602_I2C_INSTANCE    i2c0
#define LCD1602_I2C_SDA_PIN     4
#define LCD1602_I2C_SCL_PIN     5
#define LCD1602_I2C_ADDR        0x27

#define SSD1306_I2C_INSTANCE    i2c0
#define SSD1306_I2C_SDA_PIN     4
#define SSD1306_I2C_SCL_PIN     5
#define SSD1306_OLED_ADDR       0x3C

lcd_1602_i2c_t my_lcd1602;
uint8_t ssd1306_frame_buffer[SSD1306_BUF_LEN];
struct render_area ssd1306_full_frame_area = {
    .start_col = 0,
    .end_col = SSD1306_WIDTH - 1,
    .start_page = 0,
    .end_page = SSD1306_NUM_PAGES - 1
};

void vLcd1602Task(void *pvParameters) {
    lcd_init(&my_lcd1602, LCD1602_I2C_INSTANCE, LCD1602_I2C_ADDR);
    printf("LCD 1602 initialization completed in task.\n");

    lcd_clear(&my_lcd1602);
    lcd_set_cursor(&my_lcd1602, 0, 0);
    lcd_write_string(&my_lcd1602, "Console > ");
    lcd_set_cursor(&my_lcd1602, 1, 0);
    lcd_write_string(&my_lcd1602, "Started");

    for (;;) {
        static int dot_state = 0;
        lcd_set_cursor(&my_lcd1602, 1, 15);
        lcd_write_char(&my_lcd1602, dot_state ? '.' : ' ');
        dot_state = !dot_state;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vSsd1306Task(void *pvParameters) {
    ssd1306_init(SSD1306_I2C_INSTANCE, SSD1306_I2C_SDA_PIN, SSD1306_I2C_SCL_PIN);
    printf("SSD1306 OLED initialization completed in task.\n");

    ssd1306_calc_render_area_buflen(&ssd1306_full_frame_area);
    memset(ssd1306_frame_buffer, 0, SSD1306_BUF_LEN);
    ssd1306_render(ssd1306_frame_buffer, &ssd1306_full_frame_area);

    if (SSD1306_HEIGHT == 64) {
        ssd1306_write_string(ssd1306_frame_buffer, 0, 32, "BlackzCoreOS");
        ssd1306_write_string(ssd1306_frame_buffer, 0, 40, "Loading...");
    } else {
        ssd1306_write_string(ssd1306_frame_buffer, 0, 0, "BlackzCoreOS");
        ssd1306_write_string(ssd1306_frame_buffer, 0, 8, "Loading...");
    }
    ssd1306_render(ssd1306_frame_buffer, &ssd1306_full_frame_area);

    int counter = 0;
    for (;;) {
        int y_offset = (SSD1306_HEIGHT == 64) ? 40 : 8;
        memset(ssd1306_frame_buffer + (y_offset / SSD1306_PAGE_HEIGHT) * SSD1306_WIDTH, 0, SSD1306_WIDTH);
        ssd1306_write_string(ssd1306_frame_buffer, 0, y_offset, "Loading...");

        for (int i = 0; i < (counter % 4); i++) {
            ssd1306_write_char(ssd1306_frame_buffer, 8 * 9 + i * 8, y_offset, '.');
        }
        ssd1306_render(ssd1306_frame_buffer, &ssd1306_full_frame_area);
        counter++;
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

int main(void) {
    stdio_init_all();
    printf("FreeRTOS Infinity RTS Core - Start\n");

    // I2C initialisieren (shared bus für LCD1602 & SSD1306)
    i2c_init(LCD1602_I2C_INSTANCE, 100 * 1000);
    gpio_set_function(LCD1602_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(LCD1602_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(LCD1602_I2C_SDA_PIN);
    gpio_pull_up(LCD1602_I2C_SCL_PIN);
    printf("I2C0 initialized on SDA:%d, SCL:%d\n", LCD1602_I2C_SDA_PIN, LCD1602_I2C_SCL_PIN);

    // Blink-Tasks erstellen
    if (xTaskCreate(vBlinkTaskCpp, "BlinkTask_LED25", configMINIMAL_STACK_SIZE * 3, (void *)MY_CUSTOM_LED_PIN, BLINK_TASK_PRIORITY, NULL) != pdPASS) {
        printf("Error: BlinkTask_LED25 konnte nicht erstellt werden!\n");
        while (1) {}
    }
    if (xTaskCreate(vBlinkTaskCpp, "BlinkTask_WARNLED16", configMINIMAL_STACK_SIZE * 3, (void *)WARN_LED, BLINK_TASK_PRIORITY, NULL) != pdPASS) {
        printf("Error: BlinkTask_WARNLED16 konnte nicht erstellt werden!\n");
        while (1) {}
    }

    // LCD1602-Task
    if (xTaskCreate(vLcd1602Task, "Lcd1602Task", configMINIMAL_STACK_SIZE * 6, NULL, LCD1602_TASK_PRIORITY, NULL) != pdPASS) {
        printf("Error: Lcd1602Task konnte nicht erstellt werden!\n");
        while (1) {}
    }

    // SSD1306-Task
    if (xTaskCreate(vSsd1306Task, "Ssd1306Task", configMINIMAL_STACK_SIZE * 16, NULL, SSD1306_TASK_PRIORITY, NULL) != pdPASS) {
        printf("Error: Ssd1306Task konnte nicht erstellt werden!\n");
        while (1) {}
    }

    // USB-RNDIS-Task starten
    start_usb_rndis_task();

    // Webserver-Task starten
    start_webserver_task();

    // FreeRTOS Scheduler starten
    vTaskStartScheduler();

    // Sollte niemals hier landen
    printf("Fehler: Scheduler wurde beendet!\n");
    while (1) {
        tight_loop_contents();
    }

    return 0;
}