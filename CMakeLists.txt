cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#   Project settings
project(BitMasher C ASM)
set(CMAKE_C_STANDARD 11)

set(MCPU cortex-m4)

#   Paths to the Silabs Gecko SDK and GNU ARM Compiler
set(GECKO_SDK_PATH <YOUR PATH HERE>)
set(TOOLCHAIN_PATH <YOUR PATH HERE>)
set(CMSIS_PATH <YOUR PATH HERE>)

#   Path to MW_AudioFX Audio DSP Folder
set(MW_AUDIO_FX_PATH <YOUR PATH HERE>)

add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

add_compile_options(-mcpu=${MCPU} -mthumb -gdwarf-2)
add_compile_options(-mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -fno-common -fmessage-length=0)


if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-O3)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinOptDeb")
    message(STATUS "Minimal optimization with debug symbols")
    add_compile_options(-Og -g)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g3)
endif ()

include_directories(
        ${CMSIS_PATH}/Include
        silabs_gecko_sdk/device/EFM32PG12B/Include
        silabs_gecko_sdk/emlib/include
        ${TOOLCHAIN_PATH}/arm-none-eabi/include
        ${TOOLCHAIN_PATH}/arm-none-eabi/lib/gcc/arm-none-eabi/7.2.1/include
        ${TOOLCHAIN_PATH}/arm-none-eabi/lib/gcc/arm-none-eabi/7.2.1/include-fixed
        ${MW_AUDIO_FX_PATH}/Audio/ADSPBuildingBlocks
        ${MW_AUDIO_FX_PATH}/Audio/AFXUnits
        Drivers
        src
        src/Platform
        src/Platform/Peripherals
        src/Platform/Power_Clock_Management
        src/Platform/AudioEngine
        src/Platform/Graphics
        src/Platform/Utils
        src/Platform/Tests
        src/Platform/Physics
        src/App/Assets
        src/App/Controllers
        src/App/Scenes
        src/App/Scenes/Cloud
        src/App/Scenes/Glitch
        src/App/Scenes/Menu
        src/App/Scenes/Phone
        src/App/Scenes/Unicycle
        src/App/Scenes/Settings
)

add_definitions('-D__FPU_PRESENT=1' '-DARM_MATH_CM4=1' '-DEFM32PG12B500F1024GL125=1' '-D__RUN_UNIT_TESTS')

file(GLOB_RECURSE SOURCES
        "silabs_gecko_sdk/startup_files/startup_efm32pg12b.S"
        "silabs_gecko_sdk/startup_files/system_efm32pg12b.c"
        "silabs_gecko_sdk/emlib/src/*.c"
        "Drivers/MW_Driver_LS013B7DH03.c"
        "Drivers/MW_Driver_CS42L52.c"
        "src/*.c"
        "src/Platform/*.c"
        "src/Platform/Peripherals/*.c"
        "src/Platform/Power_Clock_Management/*.c"
        "src/Platform/AudioEngine/*.c"
        "src/Platform/Utils/*.c"
        "src/Platform/Debug/*.c"
        "src/Platform/Tests/*.c"
        "src/Platform/Physics/*.c"
        "src/Platform/Graphics/*.c"
        "src/Assets/*.c"
        "src/Controllers/*.c"
        "src/Scenes/*.c"
        "src/Scenes/Cloud/*.c"
        "src/Scenes/Glitch/*.c"
        "src/Scenes/Menu/*.c"
        "src/Scenes/Phone/*.c"
        "src/Scenes/Unicycle/*.c"
        "src/Scenes/Settings/*.c"
        "src/Sprites/*.c"
        ${MW_AUDIO_FX_PATH}/Audio/ADSPBuildingBlocks/*.c
        ${MW_AUDIO_FX_PATH}/Audio/AFXUnits/*.c
)

#   Linker script is provided by Silicon Labs
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/silabs_gecko_sdk/startup_files/EFM32PG12.ld)
set(ARM_MATH_LIBS arm_cortexM4lf_math.a)
link_directories(${GECKO_SDK_PATH}/platform/CMSIS/Lib/GCC)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork --specs=nano.specs -o ${PROJECT_NAME}.axf)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.axf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.axf ${ARM_MATH_LIBS} m.a)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.axf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.axf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.axf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")