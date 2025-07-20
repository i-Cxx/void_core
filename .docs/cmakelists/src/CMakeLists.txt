add_executable(${PROJECT_NAME}
    main.c
    usb_rndis_task.c
    webserver_task.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    FreeRTOS-Kernel
    tinyusb_device
    lwip
    blink
    lcd_1602_i2c
    ssd1306_i2c
)

pico_add_extra_outputs(${PROJECT_NAME})