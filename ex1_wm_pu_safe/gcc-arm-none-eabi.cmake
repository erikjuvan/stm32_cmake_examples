set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

########## INFO ###########
# You must have make in enviorment path

if (WIN32)
set(TOOLCHAIN_PATH C:/ST/STM32CubeIDE_1.10.1/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.0.202111181127/tools/bin)
set(EXECUTABLE_SUFFIX .exe)
endif (WIN32)

if (UNIX)
set(TOOLCHAIN_PATH /opt/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin)
set(EXECUTABLE_SUFFIX)
endif (UNIX)

set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PATH}/arm-none-eabi-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc${EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy${EXECUTABLE_SUFFIX})
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size${EXECUTABLE_SUFFIX})
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump${EXECUTABLE_SUFFIX})

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
