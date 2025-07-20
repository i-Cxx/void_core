add_library(led_lib STATIC
    Led.cpp
    led_task_wrapper.c     # <--- hier Korrektur von .cpp auf .c
)

target_include_directories(led_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(led_lib
    pico_stdlib
    hardware_gpio
    freertos_kernel 
    freertos_config          
)
