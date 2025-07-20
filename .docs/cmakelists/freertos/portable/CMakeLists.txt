add_library(freertos_kernel_port STATIC
    port.c
    mpu_wrappers_v2_asm.c
)

target_include_directories(freertos_kernel_port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Abhängigkeiten: Diese Bibliothek nutzt Pico SDK und Hardware Header
target_link_libraries(freertos_kernel_port
    pico_stdlib
    hardware_gpio
    hardware_irq
    hardware_timer
)

# Header für den Port (nur Interfaces, keine Kompilierung)
add_library(freertos_kernel_port_headers INTERFACE)

target_include_directories(freertos_kernel_port_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)