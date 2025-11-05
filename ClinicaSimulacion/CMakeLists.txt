cmake_minimum_required(VERSION 3.21)

# 1. Definir el proyecto
project(ClinicaSimulacion)

# 2. Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Encontrar todos nuestros archivos .cpp
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 4. CREAR EL OBJETIVO (El ejecutable)
# Ahora el objetivo "ClinicaSimulacion" existe
add_executable(${PROJECT_NAME} ${SOURCES})

# 5. MODIFICAR EL OBJETIVO (Agregar los directorios .h)
# Como el objetivo ya existe, este comando SÍ funcionará
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)