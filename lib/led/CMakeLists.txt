add_library(led_lib STATIC
    led.cpp
    led_task_wrapper.cpp
)

target_include_directories(blink_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(blink_lib
    pico_stdlib
    hardware_gpio
    freertos          
)