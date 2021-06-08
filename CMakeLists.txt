cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "PI_PICO_Si7021")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${projname}
si7021.c
        )

target_include_directories(${projname} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${projname} pico_stdlib hardware_i2c)
pico_add_extra_outputs(${projname})
# add url via pico_set_program_url

# pico_enable_stdio_usb(${projname} 1)
# pico_enable_stdio_uart(${projname} 0)
pico_add_extra_outputs(${projname})

# add url via pico_set_program_url