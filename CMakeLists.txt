# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == Configuração do Raspberry Pi Pico ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(SemaforoTemporizador2 C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o arquivo do código-fonte
add_executable(SemaforoTemporizador2 main.c)

# Define o nome e a versão do programa
pico_set_program_name(SemaforoTemporizador2 "SemaforoTemporizador2")
pico_set_program_version(SemaforoTemporizador2 "0.1")

# Habilita a comunicação via UART e USB
pico_enable_stdio_uart(SemaforoTemporizador2 1)
pico_enable_stdio_usb(SemaforoTemporizador2 1)

# Adiciona as bibliotecas necessárias
target_link_libraries(SemaforoTemporizador2
    pico_stdlib
    hardware_timer   # Adiciona suporte a temporizadores
    hardware_gpio    # Adiciona suporte a GPIO e interrupções
)

# Adiciona os diretórios de cabeçalhos
target_include_directories(SemaforoTemporizador2 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Gera os arquivos de saída extras (UF2, BIN, HEX)
pico_add_extra_outputs(SemaforoTemporizador2)
