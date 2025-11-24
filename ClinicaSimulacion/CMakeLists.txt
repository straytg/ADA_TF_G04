cmake_minimum_required(VERSION 3.10)

project(ClinicaSimulacion)

set(CMAKE_CXX_STANDARD 17)

# Incluir tus headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# === Ejecutable de CONSOLA ===
add_executable(ClinicaSimulacion
    src/main.cpp
    src/implementations/AsignadorBalanceado.cpp
    src/implementations/RegistroEficiente.cpp
    src/implementations/SchedulerOptimizado.cpp
)

# === Ejecutable WEB ===
add_executable(ClinicaWeb
    src/server.cpp
    src/implementations/AsignadorBalanceado.cpp
    src/implementations/RegistroEficiente.cpp
    src/implementations/SchedulerOptimizado.cpp
)
