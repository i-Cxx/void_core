add_library(led_lib STATIC
    led.cpp
    led_task_wrapper.cpp
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