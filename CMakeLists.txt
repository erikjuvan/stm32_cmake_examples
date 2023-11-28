cmake_minimum_required(VERSION 3.23)
project(pu_safe VERSION 1.0)

set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/linker/PU_SAFE_STM32G0B1RETX_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Executable files
add_executable(${EXECUTABLE})

# Sources
target_sources(${EXECUTABLE} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_crc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_wwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c

    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLclockrun.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLclockstart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLcrc32Run.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLmain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLstartup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/src/stm32xx_STLtranspRam.c

    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/stm32g0xx_STLRamMcMxGCC.s
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/stm32g0xx_STLcpurunGCC.s
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/stm32g0xx_STLcpustartGCC.s
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/stm32xx_STLapp.c

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Startup/startup_stm32g0b1retx.s

    ${CMAKE_CURRENT_SOURCE_DIR}/src/drain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/emrgcy_drain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/eps.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/func_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mcu_prph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/printf_support.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pu_safe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/safe_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32g0xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32g0xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system_stm32g0xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_switch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trace_support.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/valve.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/watchdog.c

    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel/black_channel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel/crc_16_arc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel_ui/ui_black_channel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel_ui/ui_crc_16_arc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/trace/src/trace.c
)

# Add header directories (AFTER add_executable !!)
target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL
    ${CMAKE_CURRENT_SOURCE_DIR}/src/STL/X_CUBE_CLASSB_V2_3_0/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/CMSIS/Device/ST/STM32G0xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/black_channel_ui
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/trace/src
)

set(MCU_MODEL STM32G0B1xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m0plus
    -mthumb
    -mfloat-abi=soft)

# Embedded macros(defines)
target_compile_definitions(${EXECUTABLE} PRIVATE ${MCU_MODEL} USE_HAL_DRIVER BLACKCH_DEVICE_MASTER LOG_VIA_UART
	$<IF:$<CONFIG:Debug>, DEBUG, NDEBUG> # NDEBUG will be used if CMAKE_BUILD_TYPE is not set. This way "Release" is default.
	$<$<CONFIG:AnotherBuildConfig>:SOME_MACRO>
	)

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    --specs=nano.specs
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<IF:$<CONFIG:Debug>,-Og -ggdb,-O2> # -O2 will be used if CMAKE_BUILD_TYPE is not set. You can think of it as "Release" being the default.
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${EXECUTABLE}.map
    --specs=nano.specs
    -Wl,--gc-sections
    -static
    -Wl,--print-memory-usage
)

target_link_libraries(${EXECUTABLE} PRIVATE
    -Wl,--start-group # It is for resolving circular dependences between several libraries
    -lc
    -lm
    -Wl,--end-group   # It is for resolving circular dependences between several libraries
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -h -S  pu_safe.elf  > "pu_safe.list"
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
    COMMAND python ../../scripts/calc_crc.py ${EXECUTABLE}.bin
    COMMAND python ../../scripts/add_crc.py ${EXECUTABLE}
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}_crc.hex
)

