# FreeRTOS Kernel + Pico Port

set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel)
set(PICO_FREERTOS_PORT_PATH ${PICO_SDK_PATH}/src/rp2_common/pico_freertos)

# FreeRTOS Kernel-Implementierung
add_library(freertos_kernel
    ${FREERTOS_KERNEL_PATH}/list.c
    ${FREERTOS_KERNEL_PATH}/queue.c
    ${FREERTOS_KERNEL_PATH}/tasks.c
    ${FREERTOS_KERNEL_PATH}/timers.c
    ${FREERTOS_KERNEL_PATH}/event_groups.c
    ${FREERTOS_KERNEL_PATH}/stream_buffer.c
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
    #${PICO_FREERTOS_PORT_PATH}/entry.c
)

target_include_directories(freertos_kernel PUBLIC
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
    ${PICO_FREERTOS_PORT_PATH}/include
    ${CMAKE_SOURCE_DIR}/include
)

# FreeRTOSConfig-Definition (Interface)
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(freertos_config INTERFACE
    projCOVERAGE_TEST=0
)
