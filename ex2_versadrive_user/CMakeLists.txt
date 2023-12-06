cmake_minimum_required(VERSION 3.10)

project(versadrive_user)

if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Please specify build configuration using -DCMAKE_BUILD_TYPE=...")
endif()

set(EXECUTABLE ${CMAKE_PROJECT_NAME}_${CMAKE_BUILD_TYPE})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if (${CMAKE_BUILD_TYPE} MATCHES "_491")
    set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/app/STM32G431CBTX_FLASH_491.ld)
    set(DEBUG_SETTING DEBUG)
    # Extract motor type from build type if for 491. Basically remove the _491 part.
    string(FIND "${CMAKE_BUILD_TYPE}" "_491" position)
    string(SUBSTRING "${CMAKE_BUILD_TYPE}" 0 ${position} MOTOR_TYPE)

    set(ADDITIONAL_SOURCES_FOR_491
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/canopen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/canopen_drv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_cobhandler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_commtask.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_direct_read.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_emcy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_errctrl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_event.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_led.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_lss.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_lssmaster.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_nmt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_nmtmaster.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_odaccess.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_pdo.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_sdoclient.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_sdoqueue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_sdoserv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_sleep.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_store.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_sync.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_timer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/codrv_error.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/canopen_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/osci/osci/osci.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/osci/osci/osci_callbacks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/osci/osci/osci_parameters.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/osci/osci/osci_ram.c
    )

else()
    set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/app/STM32G431CBTX_FLASH_431.ld)
    set(DEBUG_SETTING NDEBUG)
    set(MOTOR_TYPE ${CMAKE_BUILD_TYPE})
    set(ADDITIONAL_SOURCES_FOR_491 ) # empty
endif()

# Executable files
add_executable(${EXECUTABLE})

# Sources
target_sources(${EXECUTABLE} PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/startup_stm32g431cbtx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/IncludeBin.s

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Communication/forward.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Communication/serialprocess.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Communication/wifi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Netbuf/netbuf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Platform/platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Platform/proc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Platform/serialport.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/Platform/timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_ApplicationL.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_Base.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_LinkL.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_Mapping_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_NetworkL.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense/hisense/hisense_TransmissionL.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/motor_control/motor_control/current_rec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/motor_control/motor_control/motor_control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/motor_control/motor_control/motor_control_fsm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/motor_control/motor_control/protections.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/motor_control/motor_control/protections_obj.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/build_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/communication/mat/mat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/communication/mat/mat_requests.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/communication/mat/mat_trace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/fifobuffer/fifobuffer/fifobuffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/func_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense/hisense_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense/hisense_app_mapping.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense/hisense_proto.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/mains_meas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/measurements.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/performance_meas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/precharge.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/adc/adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/can/can/can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/comparator/comparator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/dac/dac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/dma/dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/exti/exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/gpio/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/opamp/opamp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/timer/timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/uart/uart/uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/uart/uart_cfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral/watchdog/watchdog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/relay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/stm32g4xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/stm32g4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/system.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/system_stm32g4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/trace/trace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/version.c

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen/co_direct_read.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CO_Dictionary/${CMAKE_BUILD_TYPE}/co_init.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CO_Dictionary/${CMAKE_BUILD_TYPE}/gen_objdict_variables.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CO_Dictionary/${CMAKE_BUILD_TYPE}/gen_objdict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/versadrive_simulink_generated/motor_variants/emDriveFOC_control_param_${MOTOR_TYPE}.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/versadrive_simulink_generated/simulink_model/emDriveFOC_control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/versadrive_simulink_generated/simulink_model/rtmodel.c

    ${ADDITIONAL_SOURCES_FOR_491}
)

# Add header directories (AFTER add_executable !!)
target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers/STM32G4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/versadrive_shared
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/versadrive_simulink_generated
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/peripheral
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/hisense/cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/fifobuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/osci
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/Hisense
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CANopen
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Middlewares/Protocol/CANopen/CO_Dictionary/${CMAKE_BUILD_TYPE}
)

# Embedded macros(defines)
target_compile_definitions(${EXECUTABLE} PRIVATE
    STM32G431xx USE_HAL_DRIVER BLACKCH_DEVICE_SLAVE ${DEBUG_SETTING} MOTOR_${MOTOR_TYPE}
    $<$<CONFIG:GMCC_75>:      SW_MODEL_ID=1>
    $<$<CONFIG:GMCC_75_491>:  SW_MODEL_ID=2>
    $<$<CONFIG:GMCC_66>:      SW_MODEL_ID=3>
    $<$<CONFIG:GMCC_66_491>:  SW_MODEL_ID=4>
    $<$<CONFIG:RECHI_81>:     SW_MODEL_ID=5>
    $<$<CONFIG:RECHI_81_491>: SW_MODEL_ID=6>
)

set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    )

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    --specs=nano.specs
    -Wall
    #-Wextra
    #-Wpedantic
    -Wno-unused-parameter
    $<IF:$<CONFIG:Debug>,-Og -ggdb,-Os -ggdb> # -Os will be used since Debug is not really an option. Just leaving it here as an example
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

# This does not appear to be needed, but I'm keeping it anyway for now.
target_link_libraries(${EXECUTABLE} PRIVATE
    -Wl,--start-group # It is for resolving circular dependencies between several libraries
    -lc
    -lm
    -Wl,--end-group   # It is for resolving circular dependencies between several libraries
)

add_custom_target(prebuild
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/pre_build.sh ${PROJECT_NAME} ${CMAKE_BUILD_TYPE} ${CMAKE_HOST_SYSTEM_NAME}
    COMMENT "Pre build step"
    VERBATIM
)

add_dependencies(${EXECUTABLE} prebuild)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
    COMMAND ${CMAKE_OBJDUMP} -h -S $<TARGET_FILE:${EXECUTABLE}> > ${EXECUTABLE}.list
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/post_build.sh ${EXECUTABLE}
    COMMENT "Post build step"
    VERBATIM
)

