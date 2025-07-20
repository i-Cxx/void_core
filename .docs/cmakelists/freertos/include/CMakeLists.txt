add_library(freertos_kernel_include INTERFACE)

target_include_directories(freertos_kernel_include INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(freertos_kernel_include
    INTERFACE
        $<$<TARGET_EXISTS:freertos_config>:freertos_config>
)